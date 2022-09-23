#include "minsc_test/util/str_esc.h"

#include <buf/buf.h>
#include <stddef.h>
#include <str/str.h>

typedef BUF(char) CharBuf;

str str_esc(str s) {
    CharBuf buf = BUF_NEW;
    for (size_t i = 0; i < s.len; i++) {
        char c = s.ptr[i];
        switch (c) {
            case '\0':
                BUF_PUSH(&buf, '\\');
                BUF_PUSH(&buf, '0');
                break;
            case '\a':
                BUF_PUSH(&buf, '\\');
                BUF_PUSH(&buf, 'a');
                break;
            case '\b':
                BUF_PUSH(&buf, '\\');
                BUF_PUSH(&buf, 'b');
                break;
            case '\f':
                BUF_PUSH(&buf, '\\');
                BUF_PUSH(&buf, 'f');
                break;
            case '\n':
                BUF_PUSH(&buf, '\\');
                BUF_PUSH(&buf, 'n');
                break;
            case '\r':
                BUF_PUSH(&buf, '\\');
                BUF_PUSH(&buf, 'r');
                break;
            case '\t':
                BUF_PUSH(&buf, '\\');
                BUF_PUSH(&buf, 't');
                break;
            case '\v':
                BUF_PUSH(&buf, '\\');
                BUF_PUSH(&buf, 'v');
                break;
            case '\\':
                BUF_PUSH(&buf, '\\');
                BUF_PUSH(&buf, '\\');
                break;
            case '\'':
                BUF_PUSH(&buf, '\\');
                BUF_PUSH(&buf, '\'');
                break;
            case '\"':
                BUF_PUSH(&buf, '\\');
                BUF_PUSH(&buf, '\"');
                break;
            default:
                BUF_PUSH(&buf, c);
                break;
        }
    }
    BUF_PUSH(&buf, '\0');
    return str_acquire_chars(buf.ptr, buf.len);
}
