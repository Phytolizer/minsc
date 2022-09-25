#include "string_builder.h"

StringBuilder string_builder_new(void) {
    return (StringBuilder)BUF_NEW;
}

void string_builder_clear(StringBuilder* builder) {
    for (uint64_t i = 0; i < builder->len; i++) {
        str_free(builder->ptr[i]);
    }
    builder->len = 0;
}

void string_builder_free(StringBuilder builder) {
    string_builder_clear(&builder);
    BUF_FREE(builder);
}

void string_builder_add(StringBuilder* builder, str s) {
    BUF_PUSH(builder, s);
}

str string_builder_build(StringBuilder builder) {
    return str_cat_ret_range(builder.ptr, builder.len);
}
