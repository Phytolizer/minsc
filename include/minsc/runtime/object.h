#ifndef MINSC_RUNTIME_OBJECT_H_
#define MINSC_RUNTIME_OBJECT_H_

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

typedef struct ObjectI64 {
    Object base;
    int64_t value;
} ObjectI64;

typedef struct ObjectBool {
    Object base;
    bool value;
} ObjectBool;

Object* object_new_i64(int64_t value);
Object* object_new_bool(bool value);
Object* object_dup(const Object* object);
void object_free(Object* object);

int64_t object_as_i64(const Object* object);
bool object_as_bool(const Object* object);

// Also frees the object.
int64_t object_unwrap_i64(Object* object);
bool object_unwrap_bool(Object* object);

str object_string(Object* object);

str object_type_string(ObjectType type);

#endif  // MINSC_RUNTIME_OBJECT_H_
