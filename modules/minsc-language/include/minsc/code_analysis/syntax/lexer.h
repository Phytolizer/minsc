#ifndef MINSC_CODE_ANALYSIS_SYNTAX_LEXER_H_
#define MINSC_CODE_ANALYSIS_SYNTAX_LEXER_H_

#include <str/str.h>

#include "diagnostic.h"
#include "syntax_token.h"

typedef struct Lexer Lexer;

Lexer* lexer_new(str source);
void lexer_free(Lexer* lexer);
DiagnosticBuf lexer_take_diagnostics(Lexer* lexer);

SyntaxToken* lexer_next_token(Lexer* lexer);

#endif  // MINSC_CODE_ANALYSIS_SYNTAX_LEXER_H_
