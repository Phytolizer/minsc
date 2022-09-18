#ifndef HASH_HASH_H_
#define HASH_HASH_H_

#include <println/println.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#ifndef HASH_MAX_LOAD
#define HASH_MAX_LOAD 0.75
#endif

uint64_t hash_djb2(const uint8_t* mem, size_t len);

#define HASH_ASSERT(x) \
    do { \
        if (!(x)) { \
            fprintfln(stderr, "%s: Assertion failed: " #x, __func__); \
            abort(); \
        } \
    } while (0)

#define HASH_STRING_HASH_TYPE(name, T) \
    struct name##Bucket { \
        str key; \
        T value; \
    }; \
    struct name { \
        struct name##Bucket* buckets; \
        uint64_t count; \
        uint64_t capacity; \
    };

#define HASH_STRING_HASH_NEW \
    { .buckets = NULL, .count = 0, .capacity = 0 }

#define HASH_STRING_HASH_FREE(hash, value_destructor) \
    do { \
        for (uint64_t i = 0; i < (hash).capacity; i++) { \
            if (!str_is_empty((hash).buckets[i].key)) { \
                str_free((hash).buckets[i].key); \
                value_destructor((hash).buckets[i].value); \
            } \
        } \
        free((hash).buckets); \
    } while (0)

#define HASH_STRING_HASH_INSERT(name, hash, k, v, value_destructor) \
    do { \
        while ((hash)->count + 1 >= (hash)->capacity * HASH_MAX_LOAD) { \
            uint64_t new_capacity = (hash)->capacity * 2 + 1; \
            struct name##Bucket* new_buckets = malloc(new_capacity * sizeof(struct name##Bucket)); \
            HASH_ASSERT(new_buckets != NULL); \
            for (uint64_t i = 0; i < new_capacity; i++) { \
                new_buckets[i].key = str_null; \
            } \
            for (uint64_t i = 0; i < (hash)->capacity; i++) { \
                if (str_is_empty((hash)->buckets[i].key)) { \
                    continue; \
                } \
                uint64_t h = hash_djb2( \
                    (const uint8_t*)str_ptr((hash)->buckets[i].key), \
                    str_len((hash)->buckets[i].key) \
                ); \
                uint64_t index = h % new_capacity; \
                while (!str_is_empty(new_buckets[index].key)) { \
                    index = (index + 1) % new_capacity; \
                } \
                new_buckets[index] = (hash)->buckets[i]; \
            } \
            free((hash)->buckets); \
            (hash)->buckets = new_buckets; \
            (hash)->capacity = new_capacity; \
        } \
        uint64_t h = hash_djb2((uint8_t*)str_ptr(k), str_len(k)); \
        uint64_t index = h % (hash)->capacity; \
        while (!str_is_empty((hash)->buckets[index].key) && !str_eq((hash)->buckets[index].key, k) \
        ) { \
            index = (index + 1) % (hash)->capacity; \
        } \
        if (!str_is_empty((hash)->buckets[index].key)) { \
            str_free((hash)->buckets[index].key); \
            value_destructor((hash)->buckets[index].value); \
        } else { \
            (hash)->count++; \
        } \
        (hash)->buckets[index].key = k; \
        (hash)->buckets[index].value = v; \
        (hash)->count++; \
    } while (0)

#define HASH_STRING_HASH_FIND(name, hash, k, pval) \
    do { \
        if ((hash)->capacity == 0) { \
            *(pval) = NULL; \
            break; \
        } \
        uint64_t h = hash_djb2((uint8_t*)str_ptr(k), str_len(k)); \
        uint64_t index = h % (hash)->capacity; \
        *(pval) = NULL; \
        while (!str_is_empty((hash)->buckets[index].key)) { \
            if (str_len((hash)->buckets[index].key) == str_len(k) && \
                str_eq((hash)->buckets[index].key, k)) { \
                *(pval) = (hash)->buckets[index].value; \
                break; \
            } \
            index = (index + 1) % (hash)->capacity; \
        } \
    } while (0)

#endif  // HASH_HASH_H_
