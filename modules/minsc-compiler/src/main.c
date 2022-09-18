#include <buf/buf.h>
#include <inttypes.h>
#include <linenoise.h>
#include <println/println.h>
#include <stdbool.h>
#include <stdio.h>
#include <str/str.h>
#include <styler/styler.h>

#include "minsc/code_analysis/compilation.h"
#include "minsc/code_analysis/diagnostic_bag.h"
#include "minsc/code_analysis/evaluation_result.h"
#include "minsc/code_analysis/syntax/syntax_kind.h"
#include "minsc/code_analysis/syntax/syntax_node.h"
#include "minsc/code_analysis/syntax/syntax_token.h"
#include "minsc/code_analysis/syntax/syntax_tree.h"
#include "minsc/runtime/object.h"

#ifdef _WIN32
#include <windows.h>
#endif

static void pretty_print(const SyntaxNode* root, str indent, bool is_last);

static void ensure_ansi_escape_sequences_work(void) {
#ifdef _WIN32
    // Enable ANSI escape sequences on Windows 10
    // https://docs.microsoft.com/en-us/windows/console/console-virtual-terminal-sequences
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    if (out == INVALID_HANDLE_VALUE) {
        fprintfln(stderr, "Failed to get stdout handle");
        exit(EXIT_FAILURE);
    }

    DWORD mode = 0;
    if (!GetConsoleMode(out, &mode)) {
        fprintfln(stderr, "Failed to get console mode");
        return;
    }

    mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    if (!SetConsoleMode(out, mode)) {
        fprintfln(stderr, "Failed to set console mode");
        return;
    }

    HANDLE in = GetStdHandle(STD_INPUT_HANDLE);
    if (in == INVALID_HANDLE_VALUE) {
        fprintfln(stderr, "Failed to get stdin handle");
        exit(EXIT_FAILURE);
    }

    DWORD in_mode = 0;
    if (!GetConsoleMode(in, &in_mode)) {
        fprintfln(stderr, "Failed to get console mode");
        return;
    }

    in_mode |= ENABLE_VIRTUAL_TERMINAL_INPUT;
    if (!SetConsoleMode(in, in_mode)) {
        fprintfln(stderr, "Failed to set console mode");
        return;
    }
#endif
}

int main(void) {
    ensure_ansi_escape_sequences_work();
    linenoiseHistoryLoad("minsc.history");

    bool show_tree = false;
    VariableMap* variables = variable_map_new();

    while (true) {
        char* raw_line = linenoise(">> ");
        if (raw_line == NULL) {
            break;
        }
        str line = str_acquire(raw_line);
        if (line.len == 0) {
            continue;
        }

        linenoiseHistoryAdd(line.ptr);

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

        SyntaxTree* program = syntax_tree_parse(str_ref(line));

        Compilation* compilation = compilation_new(program);
        EvaluationResult result = compilation_evaluate(compilation, &variables);

        if (show_tree) {
            styler_apply_style(styler_style_faint, stdout);
            styler_apply_fg(styler_fg_white, stdout);
            pretty_print((const SyntaxNode*)program->root, str_null, true);
            styler_apply_fg(styler_fg_reset, stdout);
            styler_apply_style(styler_style_reset, stdout);
            (void)fflush(stdout);
        }

        compilation_free(compilation);
        DiagnosticBag* diagnostics = result.diagnostics;

        if (!diagnostic_bag_empty(diagnostics)) {
            DiagnosticBuf view = diagnostic_bag_iter(diagnostics);

            for (uint64_t i = 0; i < view.len; i++) {
                styler_apply_style(styler_style_faint, stdout);
                styler_apply_fg(styler_fg_red, stdout);
                Diagnostic diagnostic = view.ptr[i];
                printfln(str_fmt, str_arg(diagnostic.message));
                styler_apply_fg(styler_fg_reset, stdout);
                styler_apply_style(styler_style_reset, stdout);

                str prefix = str_upto(line, diagnostic.span.start);
                str error =
                    str_substr_bounds(line, diagnostic.span.start, text_span_end(diagnostic.span));
                str suffix = str_after(line, text_span_end(diagnostic.span));

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

        str_free(line);
    }

    variable_map_free(variables);
    linenoiseHistorySave("minsc.history");
}

static void pretty_print(const SyntaxNode* root, str indent, bool is_last) {
    str marker = is_last ? str_lit("└───") : str_lit("├───");
    printf(
        str_fmt str_fmt str_fmt,
        str_arg(indent),
        str_arg(marker),
        str_arg(syntax_kind_string(syntax_node_kind(root)))
    );
    if (root->type == SYNTAX_NODE_TYPE_TOKEN && ((SyntaxToken*)root)->value != NULL) {
        str value = object_string(((SyntaxToken*)root)->value);
        printf(" " str_fmt, str_arg(value));
        str_free(value);
    }
    println();

    str new_indent = str_null;
    str_cat(&new_indent, indent, is_last ? str_lit("    ") : str_lit("│   "));

    SyntaxNodeChildren children = syntax_node_children(root);
    for (size_t i = 0; i < children.len; i++) {
        pretty_print(children.ptr[i], new_indent, i == children.len - 1);
    }
    str_free(new_indent);
    BUF_FREE(children);
}
