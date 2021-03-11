#pragma once

#include "common.h"

// basic allocation / deallocation
// checking malloc
// argument may not be 0.
LK_NODISCARD LK_PUBLIC void* lk_allocate(size_t n_bytes);
// checking calloc
// arguments may not be 0.
LK_NODISCARD LK_PUBLIC void* lk_allocate_array(size_t count, size_t element_size);
// frees & zeroes pointer.
// argument may not be NULL.
LK_PUBLIC LK_PUBLIC void lk_deallocate(void** mem);

// basic memory operations

// copies memory, memory regions may overlap.
// arguments must never be 0 or NULL.
LK_PUBLIC void lk_memory_move(void* restrict to, const void* restrict from, size_t n);
// copies memory, memory regions must NOT overlap.
// arguments must never be 0 or NULL.
// returns `to`.
LK_PUBLIC void lk_memory_copy(void* restrict to, const void* restrict from, size_t n);

// fills memory range with `with`.
// arguments `start` and `n` must never be NULL or zero. `with` may be 0.
LK_PUBLIC void lk_memory_fill(void* start, size_t n, u8 with);

// zeroes a range of memory.
// arguments must not be 0 or NULL.
#define lk_memory_zero(start, n) lk_memory_fill(start, n, 0)

// compares a region of memory for equality.
// arguments must not be 0 or NULL.
LK_PUBLIC bool lk_memory_equal(const void* first, const void* second, size_t n);
