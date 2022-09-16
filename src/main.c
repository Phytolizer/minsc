#include <buf/buf.h>
#include <inttypes.h>
#include <linenoise.h>
#include <println/println.h>
#include <stdbool.h>
#include <stdio.h>
#include <str/str.h>
#include <styler/styler.h>

#include "minsc/code_analysis/diagnostic.h"
#include "minsc/code_analysis/evaluator.h"
#include "minsc/code_analysis/syntax_kind.h"
#include "minsc/code_analysis/syntax_node.h"
#include "minsc/code_analysis/syntax_token.h"
#include "minsc/code_analysis/syntax_tree.h"
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

    while (true) {
        char* raw_line = linenoise(">> ");
        if (raw_line == NULL) {
            break;
        }
        str line = str_acquire(raw_line);

        linenoiseHistoryAdd(line.ptr);

        if (str_eq(line, str_lit("#showTree"))) {
            str_free(line);
            show_tree = !show_tree;
            printfln("%s",
                     show_tree ? "Showing parse trees."
                               : "Not showing parse trees.");
            continue;
        }
        if (str_eq(line, str_lit("#cls"))) {
            str_free(line);
            printf("\x1b[2J\x1b[H");
            (void)fflush(stdout);
            continue;
        }

        SyntaxTree* program = syntax_tree_parse(str_ref(line));
        if (show_tree) {
            styler_apply_style(styler_style_faint, stdout);
            styler_apply_fg(styler_fg_white, stdout);
            pretty_print((const SyntaxNode*)program->root, str_null, true);
            styler_apply_fg(styler_fg_reset, stdout);
            styler_apply_style(styler_style_reset, stdout);
            (void)fflush(stdout);
        }

        DiagnosticBuf diagnostics = program->diagnostics;
        if (diagnostics.len > 0) {
            styler_apply_style(styler_style_faint, stdout);
            styler_apply_fg(styler_fg_red, stdout);
            for (uint64_t i = 0; i < diagnostics.len; i++) {
                printfln(str_fmt, str_arg(diagnostics.ptr[i]));
            }
            styler_apply_fg(styler_fg_reset, stdout);
            styler_apply_style(styler_style_reset, stdout);
            (void)fflush(stdout);
        } else {
            Evaluator* evaluator = evaluator_new(program->root);
            int64_t result = evaluator_evaluate(evaluator);
            evaluator_free(evaluator);

            styler_apply_fg(styler_fg_green, stdout);
            printfln("%" PRId64, result);
            styler_apply_fg(styler_fg_reset, stdout);
            (void)fflush(stdout);
        }

        syntax_tree_free(program);
        str_free(line);
    }

    linenoiseHistorySave("minsc.history");
}

static void pretty_print(const SyntaxNode* root, str indent, bool is_last) {
    str marker = is_last ? str_lit("└───") : str_lit("├───");
    printf(str_fmt str_fmt str_fmt,
           str_arg(indent),
           str_arg(marker),
           str_arg(syntax_kind_string(syntax_node_kind(root))));
    if (root->type == SYNTAX_NODE_TYPE_TOKEN &&
        ((SyntaxToken*)root)->value != NULL) {
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
