#ifndef MINSC_CODE_ANALYSIS_EVALUATOR_H_
#define MINSC_CODE_ANALYSIS_EVALUATOR_H_

#include "binding/bound_expression.h"
#include "minsc/runtime/object.h"

typedef struct Evaluator Evaluator;

Evaluator* evaluator_new(BoundExpression* root);
void evaluator_free(Evaluator* evaluator);

Object* evaluator_evaluate(Evaluator* evaluator);

#endif  // MINSC_CODE_ANALYSIS_EVALUATOR_H_
