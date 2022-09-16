#ifndef MINSC_EVALUATOR_H_
#define MINSC_EVALUATOR_H_

#include <stdint.h>

#include "syntax/expression_syntax.h"

typedef struct Evaluator Evaluator;

Evaluator* evaluator_new(ExpressionSyntax* root);
void evaluator_free(Evaluator* evaluator);

int64_t evaluator_evaluate(Evaluator* evaluator);

#endif  // MINSC_EVALUATOR_H_
