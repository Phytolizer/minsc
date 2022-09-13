#pragma once

#include <stdint.h>

#include "str/str.h"

#define STRTOX_RESULT(T) \
    struct { \
        T value; \
        const char* endptr; \
        int err; \
    }

typedef STRTOX_RESULT(uint64_t) Str2U64Result;

Str2U64Result str2u64(str s, int base);
