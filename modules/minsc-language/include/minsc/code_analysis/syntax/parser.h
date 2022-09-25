#ifndef MINSC_CODE_ANALYSIS_SYNTAX_PARSER_H_
#define MINSC_CODE_ANALYSIS_SYNTAX_PARSER_H_

#include <str/str.h>

#include "minsc/code_analysis/diagnostic_bag.h"
#include "minsc/code_analysis/syntax/syntax_tree.h"
#include "minsc/code_analysis/text/source_text.h"

typedef struct Parser Parser;

Parser* parser_new(SourceText text);
void parser_free(Parser* parser);

SyntaxTree* parser_parse(Parser* parser);
DiagnosticBag* parser_take_diagnostics(Parser* parser);

#endif  // MINSC_CODE_ANALYSIS_SYNTAX_PARSER_H_
