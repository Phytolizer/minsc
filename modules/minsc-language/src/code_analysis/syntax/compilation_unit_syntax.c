#include "minsc/code_analysis/syntax/compilation_unit_syntax.h"

#include "minsc/support/minsc_assert.h"

CompilationUnit*
compilation_unit_new(ExpressionSyntax* expression, SyntaxToken* end_of_file_token) {
    CompilationUnit* unit = malloc(sizeof(CompilationUnit));
    MINSC_ASSERT(unit != NULL);
    unit->node.type = SYNTAX_NODE_TYPE_COMPILATION_UNIT;
    unit->expression = expression;
    unit->end_of_file_token = end_of_file_token;
    return unit;
}

void compilation_unit_free(CompilationUnit* unit) {
    expression_syntax_free(unit->expression);
    syntax_token_free(unit->end_of_file_token);
    free(unit);
}

SyntaxNodeChildren compilation_unit_children(const CompilationUnit* unit) {
    SyntaxNodeChildren children = BUF_NEW;
    BUF_PUSH(&children, (SyntaxNode*)unit->expression);
    BUF_PUSH(&children, (SyntaxNode*)unit->end_of_file_token);
    return children;
}
