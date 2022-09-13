#include <linenoise.h>
#include <println/println.h>
#include <stdbool.h>
#include <stdlib.h>
#include <str/str.h>
#include <string.h>

#include "lexer.h"

int main(void) {
    linenoiseHistoryLoad("minsc.history");

    while (true) {
        char* raw_line = linenoise(">> ");
        if (raw_line == NULL) {
            break;
        }
        str line = str_acquire(raw_line);

        linenoiseHistoryAdd(line.ptr);

        Lexer* lexer = lexer_new(str_ref(line));

        while (true) {
            SyntaxToken token = lexer_next_token(lexer);
            if (token.kind == SYNTAX_KIND_END_OF_FILE_TOKEN) {
                syntax_token_free(token);
                break;
            }
            printf(str_fmt ": '" str_fmt "'",
                str_arg(syntax_kind_string(token.kind)),
                str_arg(token.text));
            if (token.value != NULL) {
                str value = object_string(token.value);
                printf(" " str_fmt, str_arg(value));
                str_free(value);
            }
            printf("\n");
            syntax_token_free(token);
        }

        lexer_free(lexer);
        str_free(line);
    }

    linenoiseHistorySave("minsc.history");
}
