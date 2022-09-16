#include "minsc/runtime/object.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <str/str.h>

#include "minsc/support/minsc_assert.h"

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
