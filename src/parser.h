#ifndef MINSC_PARSER_H_
#define MINSC_PARSER_H_

#include <str/str.h>

#include "diagnostic.h"
#include "expression_syntax.h"

typedef struct Parser Parser;

Parser* parser_new(str text);
void parser_free(Parser* parser);

ExpressionSyntax* parser_parse(Parser* parser);
DiagnosticBuf parser_take_diagnostics(Parser* parser);

#endif  // MINSC_PARSER_H_
