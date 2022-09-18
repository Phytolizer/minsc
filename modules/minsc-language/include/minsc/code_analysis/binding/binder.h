#ifndef MINSC_CODE_ANALYSIS_BINDING_BINDER_H_
#define MINSC_CODE_ANALYSIS_BINDING_BINDER_H_

#include "minsc/code_analysis/binding/bound_expression.h"
#include "minsc/code_analysis/diagnostic_bag.h"
#include "minsc/code_analysis/syntax/expression_syntax.h"
#include "minsc/runtime/variable_map.h"

typedef struct Binder Binder;

Binder* binder_new(VariableMap** variables);
void binder_free(Binder* binder);

BoundExpression* binder_bind_expression(Binder* binder, ExpressionSyntax* expression);

DiagnosticBag* binder_take_diagnostics(Binder* binder);

#endif  // MINSC_CODE_ANALYSIS_BINDING_BINDER_H_
