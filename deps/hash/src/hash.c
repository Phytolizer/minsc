#include "hash/hash.h"

uint64_t hash_djb2(const uint8_t* mem, size_t len) {
    uint64_t hash = UINT64_C(5381);

    for (size_t i = 0; i < len; i++) {
        hash = hash * UINT64_C(33) + mem[i];
    }

    return hash;
}
