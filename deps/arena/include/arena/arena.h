#ifndef ARENA_ARENA_H_
#define ARENA_ARENA_H_

#include <stddef.h>
#include <stdint.h>

typedef struct Arena Arena;

Arena* arena_new(void);
void* arena_alloc(Arena* arena, size_t size);
void arena_free(Arena* arena);

#endif  // ARENA_ARENA_H_
