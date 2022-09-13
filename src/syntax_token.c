#include "syntax_token.h"

SyntaxToken* syntax_token_new(SyntaxKind kind,
                              size_t position,
                              str text,
                              Object* value) {
    SyntaxToken* token = malloc(sizeof(SyntaxToken));
    token->base.type = SYNTAX_NODE_TYPE_TOKEN;
    token->kind = kind;
    token->position = position;
    token->text = text;
    token->value = value;
    return token;
}

void syntax_token_free(SyntaxToken* token) {
    object_free(token->value);
    str_free(token->text);
    free(token);
}
