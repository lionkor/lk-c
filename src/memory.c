#include "lk/memory.h"

void* lk_allocate(size_t n_bytes) {
    LK_ASSERT(n_bytes != 0);
    void* mem = lk_malloc(n_bytes);
    if (!mem) {
        lk_log_error("allocate");
    }
    return mem;
}

void* lk_allocate_array(size_t count, size_t element_size) {
    LK_ASSERT(count * element_size != 0);
    void* mem = lk_calloc(count, element_size);
    if (!mem) {
        lk_log_error("allocate_array");
    }
    return mem;
}

void lk_deallocate(void** mem) {
    LK_ASSERT_NOT_NULL(mem);
    lk_free(*mem);
    *mem = NULL;
}

void lk_memory_move(void* restrict to, const void* restrict from, size_t n) {
    LK_ASSERT_NOT_NULL(to);
    LK_ASSERT_NOT_NULL(from);
    LK_ASSERT(n != 0);
    memmove(to, from, n);
}

void lk_memory_copy(void* restrict to, const void* restrict from, size_t n) {
    LK_ASSERT_NOT_NULL(to);
    LK_ASSERT_NOT_NULL(from);
    LK_ASSERT(n != 0);
    memcpy(to, from, n);
}

void lk_memory_fill(void* start, size_t n, u8 with) {
    LK_ASSERT_NOT_NULL(start);
    LK_ASSERT(n != 0);
    memset(start, with, n);
}

bool lk_memory_equal(const void* first, const void* second, size_t n) {
    LK_ASSERT_NOT_NULL(first);
    LK_ASSERT_NOT_NULL(second);
    LK_ASSERT(n != 0);
    return memcmp(first, second, n) == 0;
}

bool lk_reallocate(void** memory, size_t new_size) {
    LK_ASSERT_NOT_NULL(memory);
    LK_ASSERT(new_size != 0);
    // *memory may be NULL
    void* ret = lk_realloc(*memory, new_size);
    if (!ret) {
        lk_log_error("realloc");
        return false;
    }
    *memory = ret;
    return true;
}
