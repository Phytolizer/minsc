#include "minsc/runtime/object.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <str/str.h>

#include "minsc/support/minsc_assert.h"

static const char* const OBJECT_TYPE_STRINGS[] = {
#define X(x) #x,
#include "minsc/runtime/object_type_uppercamel.inc"
#undef X
};

static const size_t OBJECT_TYPE_STRING_LENGTHS[] = {
#define X(x) sizeof(#x) - 1,
#include "minsc/runtime/object_type_uppercamel.inc"
#undef X
};

Object* object_new_i64(int64_t value) {
    ObjectI64* object = malloc(sizeof(ObjectI64));
    MINSC_ASSERT(object != NULL);
    object->base.type = OBJECT_TYPE_INT64;
    object->value = value;
    return (Object*)object;
}

Object* object_new_bool(bool value) {
    ObjectBool* object = malloc(sizeof(ObjectBool));
    MINSC_ASSERT(object != NULL);
    object->base.type = OBJECT_TYPE_BOOL;
    object->value = value;
    return (Object*)object;
}

Object* object_dup(const Object* object) {
    if (object == NULL) {
        return NULL;
    }
    switch (object->type) {
        case OBJECT_TYPE_NULL:
            MINSC_ABORT("Corrupt object: non-NULL object with type NULL");
        case OBJECT_TYPE_INT64:
            return object_new_i64(((ObjectI64*)object)->value);
        case OBJECT_TYPE_BOOL:
            return object_new_bool(((ObjectBool*)object)->value);
    }
    return NULL;
}

void object_free(Object* object) {
    free(object);
}

bool object_eq(Object* a, Object* b) {
    if (a == NULL || b == NULL) {
        return a == b;
    }
    if (a->type != b->type) {
        return false;
    }
    switch (a->type) {
        case OBJECT_TYPE_NULL:
            MINSC_ABORT("Corrupt object: non-NULL object with type NULL");
        case OBJECT_TYPE_INT64:
            return ((ObjectI64*)a)->value == ((ObjectI64*)b)->value;
        case OBJECT_TYPE_BOOL:
            return ((ObjectBool*)a)->value == ((ObjectBool*)b)->value;
        default:
            MINSC_ABORT("Corrupt object: unknown type");
    }
}

int64_t object_as_i64(const Object* object) {
    MINSC_ASSERT(object != NULL);
    MINSC_ASSERT(object->type == OBJECT_TYPE_INT64);
    return ((ObjectI64*)object)->value;
}

bool object_as_bool(const Object* object) {
    MINSC_ASSERT(object != NULL);
    MINSC_ASSERT(object->type == OBJECT_TYPE_BOOL);
    return ((ObjectBool*)object)->value;
}

int64_t object_unwrap_i64(Object* object) {
    MINSC_ASSERT(object != NULL);
    MINSC_ASSERT(object->type == OBJECT_TYPE_INT64);
    int64_t value = ((ObjectI64*)object)->value;
    object_free(object);
    return value;
}

bool object_unwrap_bool(Object* object) {
    MINSC_ASSERT(object != NULL);
    MINSC_ASSERT(object->type == OBJECT_TYPE_BOOL);
    bool value = ((ObjectBool*)object)->value;
    object_free(object);
    return value;
}

str object_string(Object* object) {
    switch (object->type) {
        case OBJECT_TYPE_INT64: {
            ObjectI64* i64 = (ObjectI64*)object;
            return str_printf("%" PRId64, i64->value);
        }
        case OBJECT_TYPE_BOOL: {
            ObjectBool* boolean = (ObjectBool*)object;
            return boolean->value ? str_lit("true") : str_lit("false");
        }
        default:
            MINSC_ABORT("Unhandled object type");
    }
}

str object_type_string(ObjectType type) {
    switch (type) {
#define X(x) case OBJECT_TYPE_##x:
#include "minsc/runtime/object_type.inc"
#undef X
        return str_ref_chars(OBJECT_TYPE_STRINGS[type], OBJECT_TYPE_STRING_LENGTHS[type]);
        default:
            return str_lit("OBJECT_TYPE_UNKNOWN");
    }
}
