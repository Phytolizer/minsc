#ifndef MINSC_PARSER_H_
#define MINSC_PARSER_H_

#include <str/str.h>

typedef struct Parser Parser;

Parser* parser_new(str text);
void parser_free(Parser* parser);

#endif  // MINSC_PARSER_H_
