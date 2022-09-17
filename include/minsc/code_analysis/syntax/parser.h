#ifndef MINSC_CODE_ANALYSIS_SYNTAX_PARSER_H_
#define MINSC_CODE_ANALYSIS_SYNTAX_PARSER_H_

#include <str/str.h>

#include "diagnostic.h"
#include "syntax_tree.h"

typedef struct Parser Parser;

Parser* parser_new(str text);
void parser_free(Parser* parser);

SyntaxTree* parser_parse(Parser* parser);
DiagnosticBuf parser_take_diagnostics(Parser* parser);

#endif  // MINSC_CODE_ANALYSIS_SYNTAX_PARSER_H_
