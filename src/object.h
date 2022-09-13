#ifndef MINSC_OBJECT_H_
#define MINSC_OBJECT_H_

#include <stdint.h>
#include <str/str.h>

typedef enum {
#define X(x) OBJECT_TYPE_##x,
#include "object_type.inc"
#undef X
} ObjectType;

typedef struct Object {
    ObjectType type;
} Object;

typedef struct ObjectU64 {
    Object base;
    uint64_t value;
} ObjectU64;

Object* object_new_u64(uint64_t value);
void object_free(Object* object);

str object_string(Object* object);

#endif  // MINSC_OBJECT_H_
