#include <inttypes.h>
#include <linenoise.h>
#include <println/println.h>
#include <stdbool.h>
#include <stdio.h>
#include <str/str.h>
#include <styler/styler.h>

#include "buf/buf.h"
#include "diagnostic.h"
#include "evaluator.h"
#include "object.h"
#include "parser.h"
#include "syntax_kind.h"
#include "syntax_node.h"
#include "syntax_token.h"
#include "syntax_tree.h"

static void pretty_print(const SyntaxNode* root, str indent, bool is_last);

int main(void) {
    linenoiseHistoryLoad("minsc.history");

    while (true) {
        char* raw_line = linenoise(">> ");
        if (raw_line == NULL) {
            break;
        }
        str line = str_acquire(raw_line);

        linenoiseHistoryAdd(line.ptr);

        Parser* parser = parser_new(str_ref(line));
        SyntaxTree* program = parser_parse(parser);
        parser_free(parser);
        styler_apply_style(styler_style_faint, stdout);
        styler_apply_fg(styler_fg_white, stdout);
        pretty_print((const SyntaxNode*)program->root, str_null, true);
        styler_apply_fg(styler_fg_reset, stdout);
        styler_apply_style(styler_style_reset, stdout);
        (void)fflush(stdout);

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
