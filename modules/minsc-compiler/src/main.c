#include <stddef.h>
// ^ needed for linenoise, order matters
#include <inttypes.h>
#include <linenoise.h>
#include <minsc/code_analysis/compilation.h>
#include <minsc/code_analysis/diagnostic.h>
#include <minsc/code_analysis/diagnostic_bag.h>
#include <minsc/code_analysis/evaluation_result.h>
#include <minsc/code_analysis/syntax/syntax_node.h>
#include <minsc/code_analysis/syntax/syntax_tree.h>
#include <minsc/code_analysis/text/text_span.h>
#include <minsc/runtime/object.h>
#include <minsc/runtime/variable_map.h>
#include <println/println.h>
#include <stdbool.h>
#include <stdio.h>
#include <str/str.h>
#include <styler/styler.h>

#include "string_builder.h"

#ifdef _WIN32
#include <windows.h>
#endif

static bool ensure_ansi_escape_sequences_work(void) {
#ifdef _WIN32
    // Enable ANSI escape sequences on Windows 10
    // https://docs.microsoft.com/en-us/windows/console/console-virtual-terminal-sequences
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    if (out == INVALID_HANDLE_VALUE) {
        fprintfln(stderr, "Failed to get stdout handle");
        return false;
    }

    DWORD mode = 0;
    if (!GetConsoleMode(out, &mode)) {
        fprintfln(stderr, "Failed to get console mode");
        return false;
    }

    mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    if (!SetConsoleMode(out, mode)) {
        fprintfln(stderr, "Failed to set console mode");
        return false;
    }

    HANDLE in = GetStdHandle(STD_INPUT_HANDLE);
    if (in == INVALID_HANDLE_VALUE) {
        fprintfln(stderr, "Failed to get stdin handle");
        return false;
    }

    DWORD in_mode = 0;
    if (!GetConsoleMode(in, &in_mode)) {
        fprintfln(stderr, "Failed to get console mode");
        return false;
    }

    in_mode |= ENABLE_VIRTUAL_TERMINAL_INPUT;
    if (!SetConsoleMode(in, in_mode)) {
        fprintfln(stderr, "Failed to set console mode");
        return false;
    }
#endif
    return true;
}

#ifdef _WIN32
#define STRNCPY(dst, src, n) strncpy_s(dst, n, src, n)
#else
#define STRNCPY(dst, src, n) strncpy(dst, src, n)
#endif

int main(void) {
    if (!ensure_ansi_escape_sequences_work()) {
        return 1;
    }
    linenoiseHistoryLoad("minsc.history");

    bool show_tree = false;
    VariableMap* variables = variable_map_new();
    StringBuilder text_builder = string_builder_new();

    while (true) {
        // 13 == 10 for colors + 2 for '> ' + 1 for NUL
        char prompt[13];
        int n = styler_str_fg(styler_fg_green, prompt);
        STRNCPY(&prompt[n], text_builder.len == 0 ? ">" : "|", 2);
        n += 1;
        n += styler_str_fg(styler_fg_reset, &prompt[n]);
        prompt[n++] = ' ';
        prompt[n++] = '\0';
        char* raw_line = linenoise(prompt);
        if (raw_line == NULL) {
            break;
        }
        str line = str_acquire(raw_line);
        bool is_blank = line.len == 0;

        linenoiseHistoryAdd(line.ptr);

        if (text_builder.len == 0) {
            if (str_eq(line, str_lit("#showTree"))) {
                str_free(line);
                show_tree = !show_tree;
                printfln("%s", show_tree ? "Showing parse trees." : "Not showing parse trees.");
                continue;
            }
            if (str_eq(line, str_lit("#cls"))) {
                str_free(line);
                printf("\x1b[2J\x1b[H");
                (void)fflush(stdout);
                continue;
            }
        }

        string_builder_add(&text_builder, line);
        string_builder_add(&text_builder, str_lit("\n"));
        str text = string_builder_build(text_builder);
        SyntaxTree* program = syntax_tree_parse(str_ref(text));

        if (!is_blank && !diagnostic_bag_empty(program->diagnostics)) {
            syntax_tree_free(program);
            str_free(text);
            continue;
        }

        Compilation* compilation = compilation_new(program);
        EvaluationResult result = compilation_evaluate(compilation, &variables);

        if (show_tree) {
            styler_apply_style(styler_style_faint, stdout);
            styler_apply_fg(styler_fg_white, stdout);
            syntax_node_pretty_print((const SyntaxNode*)program->root, stdout);
            styler_apply_fg(styler_fg_reset, stdout);
            styler_apply_style(styler_style_reset, stdout);
            (void)fflush(stdout);
        }

        compilation_free(compilation);
        DiagnosticBag* diagnostics = result.diagnostics;

        if (!diagnostic_bag_empty(diagnostics)) {
            DiagnosticBuf view = diagnostic_bag_iter(diagnostics);

            for (uint64_t i = 0; i < view.len; i++) {
                Diagnostic diagnostic = view.ptr[i];
                size_t line_index =
                    source_text_get_line_index(program->source, diagnostic.span.start);
                size_t line_number = line_index + 1;
                TextLine line = program->source.lines.ptr[line_index];
                size_t character = diagnostic.span.start - line.start + 1;

                styler_apply_style(styler_style_faint, stdout);
                styler_apply_fg(styler_fg_red, stdout);
                printfln(
                    "(%zu, %zu): " str_fmt,
                    line_number,
                    character,
                    str_arg(diagnostic.message)
                );
                styler_apply_fg(styler_fg_reset, stdout);
                styler_apply_style(styler_style_reset, stdout);

                str prefix = str_substr_bounds(text, line.start, diagnostic.span.start);
                str error =
                    str_substr_bounds(text, diagnostic.span.start, text_span_end(diagnostic.span));
                str suffix =
                    str_substr_bounds(text, text_span_end(diagnostic.span), text_line_end(line));

                printf("    " str_fmt, str_arg(prefix));
                styler_apply_style(styler_style_faint, stdout);
                styler_apply_fg(styler_fg_red, stdout);
                printf(str_fmt, str_arg(error));
                styler_apply_fg(styler_fg_reset, stdout);
                styler_apply_style(styler_style_reset, stdout);
                printfln(str_fmt, str_arg(suffix));
            }
            diagnostic_bag_free(diagnostics);
            (void)fflush(stdout);
        } else {
            str result_str = object_string(result.value);
            object_free(result.value);

            styler_apply_fg(styler_fg_magenta, stdout);
            printfln(str_fmt, str_arg(result_str));
            str_free(result_str);
            styler_apply_fg(styler_fg_reset, stdout);
            (void)fflush(stdout);
        }

        str_free(text);
        syntax_tree_free(program);
        string_builder_clear(&text_builder);
    }

    variable_map_free(variables);
    string_builder_free(text_builder);
    linenoiseHistorySave("minsc.history");
}
