#ifndef MINSC_MINSC_ASSERT_H_
#define MINSC_MINSC_ASSERT_H_

#include <stdlib.h>

#define MINSC_ABORT(msg) \
    do { \
        (void)fprintf(stderr, "%s: FATAL: %s\n", __func__, msg); \
        abort(); \
    } while (0)

#define MINSC_ASSERT(x) \
    do { \
        if (!(x)) { \
            MINSC_ABORT("Assertion failed: " #x); \
        } \
    } while (0)

#endif  // MINSC_MINSC_ASSERT_H_
