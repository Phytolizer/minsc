#include "arena/arena.h"

#if defined(ARENA_BACKEND_LIBC_MALLOC)
#include <stdlib.h>
#elif defined(ARENA_BACKEND_LINUX_MMAP)
#include <sys/mman.h>
#elif defined(ARENA_BACKEND_WIN32_VIRTUALALLOC)
#include <windows.h>
#elif defined(ARENA_BACKEND_WASM_HEAPBASE)
#include <emscripten.h>
#endif

typedef struct RegionHeader {
    struct RegionHeader* next;
    size_t size;
    size_t capacity;
} RegionHeader;

struct Arena {
    RegionHeader* begin;
    RegionHeader* end;
};

enum {
    REGION_DEFAULT_CAPACITY = 8 * 1024,
};

static RegionHeader* region_new(size_t capacity) {
    size_t total_size = sizeof(RegionHeader) + capacity;
#if defined(ARENA_BACKEND_LIBC_MALLOC)
    RegionHeader* region = malloc(total_size);
#elif defined(ARENA_BACKEND_LINUX_MMAP)
    RegionHeader* region =
        mmap(NULL, total_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
#elif defined(ARENA_BACKEND_WIN32_VIRTUALALLOC)
    RegionHeader* region = VirtualAlloc(NULL, total_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
#elif defined(ARENA_BACKEND_WASM_HEAPBASE)
    RegionHeader* region = (RegionHeader*)EM_ASM_INT(
        {
            var size = $0;
            var ptr = HEAP32[0];
            var end = ptr + size;
            if (end > HEAP32[1]) throw 'out of memory';
            HEAP32[0] = end;
            return ptr;
        },
        total_size
    );
#endif
    region->next = NULL;
    region->size = 0;
    region->capacity = capacity;
    return region;
}

static void free_region(RegionHeader* region) {
#if defined(ARENA_BACKEND_LIBC_MALLOC)
    free(region);
#elif defined(ARENA_BACKEND_LINUX_MMAP)
    size_t total_size = sizeof(RegionHeader) + region->capacity;
    munmap(region, total_size);
#elif defined(ARENA_BACKEND_WIN32_VIRTUALALLOC)
    VirtualFree(region, 0, MEM_RELEASE);
#elif defined(ARENA_BACKEND_WASM_HEAPBASE)
    size_t total_size = sizeof(RegionHeader) + region->capacity;
    EM_ASM(
        {
            var ptr = $0;
            var size = $1;
            var end = HEAP32[0];
            if (end != ptr + size) throw 'invalid free';
            HEAP32[0] = ptr;
        },
        region,
        total_size
    );
#endif
}

Arena* arena_new(void) {
#if defined(ARENA_BACKEND_LIBC_MALLOC)
    Arena* a = malloc(sizeof(Arena));
#elif defined(ARENA_BACKEND_LINUX_MMAP)
    Arena* a =
        mmap(NULL, sizeof(Arena), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
#elif defined(ARENA_BACKEND_WIN32_VIRTUALALLOC)
    Arena* a = VirtualAlloc(NULL, sizeof(Arena), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
#elif defined(ARENA_BACKEND_WASM_HEAPBASE)
    Arena* a = (Arena*)EM_ASM_INT(
        {
            var size = $0;
            var ptr = HEAP32[0];
            var end = ptr + size;
            if (end > HEAP32[1]) throw 'out of memory';
            HEAP32[0] = end;
            return ptr;
        },
        sizeof(Arena)
    );
#endif

    a->begin = NULL;
    a->end = NULL;
    return a;
}

void* arena_alloc(Arena* arena, size_t size) {
    if (arena->end == NULL) {
        size_t capacity = REGION_DEFAULT_CAPACITY;
        if (capacity < size) {
            capacity = size;
        }
        arena->begin = region_new(capacity);
        arena->end = arena->begin;
    }

    while (arena->end->size + size > arena->end->capacity) {
        if (arena->end->next == NULL) {
            size_t capacity = arena->end->capacity * 2;
            if (capacity < size) {
                capacity = size;
            }
            arena->end->next = region_new(capacity);
        }
        arena->end = arena->end->next;
    }

    void* ptr = (char*)arena->end + sizeof(RegionHeader) + arena->end->size;
    arena->end->size += size;
    return ptr;
}

void arena_free(Arena* arena) {
    RegionHeader* region = arena->begin;
    while (region != NULL) {
        RegionHeader* next = region->next;
        free_region(region);
        region = next;
    }

#if defined(ARENA_BACKEND_LIBC_MALLOC)
    free(arena);
#elif defined(ARENA_BACKEND_LINUX_MMAP)
    munmap(arena, sizeof(Arena));
#elif defined(ARENA_BACKEND_WIN32_VIRTUALALLOC)
    VirtualFree(arena, 0, MEM_RELEASE);
#elif defined(ARENA_BACKEND_WASM_HEAPBASE)
    EM_ASM(
        {
            var ptr = $0;
            var size = $1;
            var end = HEAP32[0];
            if (end != ptr + size) throw 'invalid free';
            HEAP32[0] = ptr;
        },
        arena,
        sizeof(Arena)
    );
#endif
}
