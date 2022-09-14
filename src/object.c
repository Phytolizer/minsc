#include "object.h"

#include <inttypes.h>
#include <println/println.h>
#include <stdlib.h>

#include "minsc_assert.h"

Object* object_new_u64(uint64_t value) {
    ObjectU64* object = malloc(sizeof(ObjectU64));
    MINSC_ASSERT(object != NULL);
    object->base.type = OBJECT_TYPE_U64;
    object->value = value;
    return (Object*)object;
}

Object* object_dup(const Object* object) {
    if (object == NULL) {
        return NULL;
    }
    switch (object->type) {
        case OBJECT_TYPE_U64:
            return object_new_u64(((ObjectU64*)object)->value);
    }
    return NULL;
}

void object_free(Object* object) {
    free(object);
}

str object_string(Object* object) {
    switch (object->type) {
        case OBJECT_TYPE_U64: {
            ObjectU64* u64 = (ObjectU64*)object;
            return str_printf("%" PRIu64, u64->value);
        }
        default:
            fprintfln(stderr, "TODO");
            abort();
    }
}
