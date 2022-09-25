#ifndef STRING_BUILDER_H_
#define STRING_BUILDER_H_

#include <buf/buf.h>
#include <str/str.h>

typedef BUF(str) StringBuilder;

StringBuilder string_builder_new(void);
void string_builder_clear(StringBuilder* builder);
void string_builder_free(StringBuilder builder);
void string_builder_add(StringBuilder* builder, str s);
str string_builder_build(StringBuilder builder);

#endif  // STRING_BUILDER_H_
