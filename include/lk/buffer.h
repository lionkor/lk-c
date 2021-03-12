#pragma once

#include "lk/common.h"
#include <limits.h>

typedef struct {
    u8* data;
    size_t size;
} LKBuffer;
typedef bool (*LKBufferForeachCallback)(u8*);
typedef bool (*LKBufferPredicate)(u8);

// dynamically allocates the buffer's data, zeroes it out
LK_PUBLIC LKBuffer lk_buffer_create(size_t size);
LK_PUBLIC void lk_buffer_destroy(LKBuffer* buf);
LK_PUBLIC LK_NODISCARD bool lk_buffer_resize(LKBuffer* buf, size_t new_size);
LK_PUBLIC void lk_buffer_zero(LKBuffer* buf);
LK_PUBLIC void lk_buffer_fill(LKBuffer* buf, u8 with);
// calls `fn` on each element in the buffer. breaks when `fn` returns false.
LK_PUBLIC void lk_buffer_foreach(LKBuffer* buf, LKBufferForeachCallback fn);
// returns index of first matching element if found, SIZE_MAX if not found.
LK_PUBLIC LK_NODISCARD size_t lk_buffer_find(LKBuffer* buf, u8 elem);
// returns index of first matching element, where "matching" means `pred(element) == true`.
// returns SIZE_MAX if no match was found.
LK_PUBLIC LK_NODISCARD size_t lk_buffer_find_if(LKBuffer* buf, LKBufferPredicate pred);
// returns index of seq in buf if found, or SIZE_MAX if not found.
LK_PUBLIC LK_NODISCARD size_t lk_buffer_search(LKBuffer* buf, u8* seq, size_t seq_len);
