#ifndef MINSC_MINSC_ASSERT_H_
#define MINSC_MINSC_ASSERT_H_

#include <stdlib.h>

#define MINSC_ASSERT(x) \
    do { \
        if (!(x)) { \
            (void)fprintf(stderr, "Assertion failed: %s\n", #x); \
            abort(); \
        } \
    } while (0)

#endif  // MINSC_MINSC_ASSERT_H_
