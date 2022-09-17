#ifndef MINSC_CODE_ANALYSIS_BINDING_BOUND_LITERAL_EXPRESSION_H_
#define MINSC_CODE_ANALYSIS_BINDING_BOUND_LITERAL_EXPRESSION_H_

#include "bound_expression.h"
#include "minsc/runtime/object.h"

typedef struct BoundLiteralExpression {
    BoundExpression base;
    Object* value;
} BoundLiteralExpression;

extern BoundExpression* bound_literal_expression_new(Object* value);
extern void bound_literal_expression_free(BoundLiteralExpression* expression);

extern ObjectType bound_literal_expression_type(const BoundLiteralExpression* expression);

#endif  // MINSC_CODE_ANALYSIS_BINDING_BOUND_LITERAL_EXPRESSION_H_
