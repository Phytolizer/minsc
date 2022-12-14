#include <arg/arg.h>
#include <arg/buf.h>
#include <arg/str.h>
#include <buf/buf.h>
#include <config.h>
#include <ctype.h>
#include <file/file.h>
#include <println/println.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <str/str.h>
#include <sum/sum.h>

#define str_from_arg_str(arg_str) str_ref_chars((arg_str).ptr, arg_str_len((arg_str)))

typedef SUM_MAYBE_TYPE(str) PathExtension;

static PathExtension extension(str path) {
    size_t last_dot;
    if (!str_find_last(path, '.', &last_dot) || last_dot == 0) {
        return (PathExtension)SUM_NOTHING;
    }

    return (PathExtension)SUM_JUST(str_after(path, last_dot + 1));
}

typedef BUF(str) XMacros;

static XMacros parse_xmacros(str source) {
    XMacros result = BUF_NEW;

    size_t oldstart = 0;
    size_t start;
    while (str_find(str_after(source, oldstart), str_lit("X("), &start)) {
        start += oldstart + 2;
        size_t end = 0;
        if (!str_find(str_after(source, start), ')', &end)) {
            fprintfln(stderr, "X macro without closing parenthesis");
            exit(EXIT_FAILURE);
        }
        end += start;
        str xmacro = str_ref_chars(source.ptr + start, end - start);
        BUF_PUSH(&result, xmacro);
        oldstart = start;
    }

    return result;
}

static void varycase_camel_internal(str ident, FILE* fp, bool upper) {
    for (size_t i = 0; i < ident.len; i++) {
        if (ident.ptr[i] == '_') {
            upper = true;
        } else if (upper) {
            (void)fputc(toupper(ident.ptr[i]), fp);
            upper = false;
        } else {
            (void)fputc(tolower(ident.ptr[i]), fp);
        }
    }
}

static void varycase_uppercamel(str ident, FILE* fp) {
    varycase_camel_internal(ident, fp, true);
}

static void varycase_camel(str ident, FILE* fp) {
    varycase_camel_internal(ident, fp, false);
}

static void varycase(str ident, str new_case, FILE* fp) {
    if (str_eq(new_case, str_lit("camel"))) {
        varycase_camel(ident, fp);
    } else if (str_eq(new_case, str_lit("uppercamel"))) {
        varycase_uppercamel(ident, fp);
    } else {
        fprintfln(stderr, "unknown case: " str_fmt, str_arg(new_case));
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char** argv) {
    Arg help_arg = ARG_FLAG(
            .shortname = 'h',
            .longname = arg_str_lit("help"),
            .help = arg_str_lit("Show this help message")
    );
    Arg case_arg = ARG_POS(arg_str_lit("CASE"), arg_str_lit("Case to convert to"));
    Arg input_arg = ARG_POS(arg_str_lit("INPUT"), arg_str_lit("String to convert"));
    Arg output_arg =
        ARG_OPT(.shortname = 'o',
                .longname = arg_str_lit("output"),
                .help = arg_str_lit("Output file (default <INPUT>_<CASE>.inc)"));
    Arg* args[] = {&help_arg, &case_arg, &input_arg, &output_arg};
    ArgParser parser = arg_parser_new(
        arg_str_lit("varycase"),
        arg_str_lit(PROJECT_DESCRIPTION),
        (ArgBuf)ARG_BUF_ARRAY(args)
    );
    ArgParseErr err = arg_parser_parse(&parser, argc, argv);
    if (help_arg.flagValue) {
        arg_parser_show_help(&parser, stdout);
        if (err.present) {
            arg_str_free(err.value);
        }
        return EXIT_SUCCESS;
    }
    ARG_BUF_FREE(parser.extra);

    if (err.present) {
        arg_parser_show_help(&parser, stderr);
        fprintfln(stderr, "ERROR: " ARG_STR_FMT, ARG_STR_ARG(err.value));
        arg_str_free(err.value);
        return EXIT_FAILURE;
    }

    if (arg_str_len(case_arg.value) == 0) {
        arg_parser_show_help(&parser, stderr);
        fprintfln(stderr, "ERROR: Missing required argument CASE");
        return EXIT_FAILURE;
    }

    if (arg_str_len(input_arg.value) == 0) {
        arg_parser_show_help(&parser, stderr);
        fprintfln(stderr, "ERROR: Missing required argument INPUT");
        return EXIT_FAILURE;
    }
    str input = str_from_arg_str(input_arg.value);

    str output = str_from_arg_str(output_arg.value);
    if (str_is_empty(output)) {
        str input_noext = input;
        PathExtension ext = extension(input_noext);
        if (ext.present) {
            input_noext = str_upto(input_noext, ext.value.ptr - input_noext.ptr - 1);
        }
        output = str_printf(
            ARG_STR_FMT "_" ARG_STR_FMT ".inc",
            str_arg(input_noext),
            ARG_STR_ARG(case_arg.value)
        );
    }

    SlurpFileResult slurp = slurp_file(input);
    if (!slurp.ok) {
        fprintfln(
            stderr,
            "ERROR: Failed to read file " str_fmt ": " str_fmt,
            str_arg(input),
            str_arg(slurp.get.error)
        );
        str_free(slurp.get.error);
        str_free(output);
        return EXIT_FAILURE;
    }

    str input_contents = slurp.get.value;

    size_t last_slash;
    if (str_find_last(output, '/', &last_slash)) {
        str dir = str_upto(output, last_slash);
        if (!file_exists(dir)) {
            FileMkdirRecError mkdir_err = file_mkdir_rec(dir);
            if (mkdir_err.present) {
                fprintfln(stderr, "ERROR: Failed to create directory " str_fmt, str_arg(dir));
                str_free(dir);
                str_free(input_contents);
                str_free(output);
                return EXIT_FAILURE;
            }
        } else if (!file_isdir(dir)) {
            fprintfln(stderr, "ERROR: " str_fmt " is not a directory", str_arg(dir));
            str_free(dir);
            str_free(input_contents);
            str_free(output);
            return EXIT_FAILURE;
        }
        str_free(dir);
    }

    XMacros xmacros = parse_xmacros(input_contents);

    FILE* output_fp = fopen(output.ptr, "w");
    if (output_fp == NULL) {
        fprintfln(stderr, "ERROR: Failed to open file " str_fmt " for writing", str_arg(output));
        BUF_FREE(xmacros);
        str_free(input_contents);
        str_free(output);
        return EXIT_FAILURE;
    }

    for (uint64_t i = 0; i < xmacros.len; i++) {
        (void)fprintf(output_fp, "X(");
        varycase(xmacros.ptr[i], str_from_arg_str(case_arg.value), output_fp);
        fprintfln(output_fp, ")");
    }

    BUF_FREE(xmacros);
    (void)fclose(output_fp);
    str_free(input_contents);
    str_free(output);
    return EXIT_SUCCESS;
}
