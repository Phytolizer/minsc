#include "syntax_token.h"

void syntax_token_free(SyntaxToken token) {
    object_free(token.value);
    str_free(token.text);
}
