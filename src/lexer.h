#ifndef MINSC_LEXER_H_
#define MINSC_LEXER_H_

#include <str/str.h>

#include "syntax_token.h"

typedef struct Lexer Lexer;

Lexer* lexer_new(str source);
void lexer_free(Lexer* lexer);

SyntaxToken lexer_next_token(Lexer* lexer);

#endif  // MINSC_LEXER_H_
