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
    object->base.type = OBJECT_TYPE_I64;
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
        case OBJECT_TYPE_I64:
            return object_new_i64(((ObjectI64*)object)->value);
    }
    return NULL;
}

void object_free(Object* object) {
    free(object);
}

int64_t object_as_i64(const Object* object) {
    MINSC_ASSERT(object != NULL);
    MINSC_ASSERT(object->type == OBJECT_TYPE_I64);
    return ((ObjectI64*)object)->value;
}

str object_string(Object* object) {
    switch (object->type) {
        case OBJECT_TYPE_I64: {
            ObjectI64* i64 = (ObjectI64*)object;
            return str_printf("%" PRIu64, i64->value);
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
        return str_ref_chars(OBJECT_TYPE_STRINGS[type],
                             OBJECT_TYPE_STRING_LENGTHS[type]);
    }
}
