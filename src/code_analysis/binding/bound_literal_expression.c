#include "minsc/code_analysis/binding/bound_literal_expression.h"

#include <stdlib.h>

#include "minsc/support/minsc_assert.h"

extern BoundExpression* bound_literal_expression_new(Object* value) {
    BoundLiteralExpression* expression = malloc(sizeof(BoundLiteralExpression));
    MINSC_ASSERT(expression != NULL);
    expression->base.base.type = BOUND_NODE_TYPE_EXPRESSION;
    expression->base.type = BOUND_EXPRESSION_TYPE_LITERAL;
    expression->value = value;
    return (BoundExpression*)expression;
}

extern void bound_literal_expression_free(BoundLiteralExpression* expression) {
    object_free(expression->value);
    free(expression);
}

extern ObjectType bound_literal_expression_type(
    const BoundLiteralExpression* expression
) {
    if (expression->value == NULL) {
        return OBJECT_TYPE_NULL;
    }
    return expression->value->type;
}
