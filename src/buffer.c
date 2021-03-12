#include "lk/buffer.h"
#include "lk/memory.h"

LKBuffer lk_buffer_create(size_t size) {
    LKBuffer buf = { .data = lk_allocate(size), .size = size };
    lk_memory_zero(buf.data, size);
    return buf;
}

void lk_buffer_destroy(LKBuffer* buf) {
    LK_ASSERT_NOT_NULL(buf);
    lk_deallocate((void**)&buf->data);
    buf->size = 0;
}

bool lk_buffer_resize(LKBuffer* buf, size_t new_size) {
    LK_ASSERT_NOT_NULL(buf);
    LK_ASSERT(new_size != 0);
    if (new_size == buf->size) {
        return true;
    }
    return lk_reallocate((void**)&buf->data, new_size);
}

void lk_buffer_zero(LKBuffer* buf) {
    LK_ASSERT_NOT_NULL(buf);
    lk_memory_zero(buf->data, buf->size);
}

void lk_buffer_fill(LKBuffer* buf, u8 with) {
    LK_ASSERT_NOT_NULL(buf);
    lk_memory_fill(buf->data, buf->size, with);
}

void lk_buffer_foreach(LKBuffer* buf, LKBufferForeachCallback fn) {
    LK_ASSERT_NOT_NULL(buf);
    LK_ASSERT_NOT_NULL(fn);
    for (size_t i = 0; i < buf->size; ++i) {
        if (!fn(&buf->data[i])) {
            break;
        }
    }
}

static inline size_t find(u8* ptr, size_t size, u8 what) {
    for (size_t i = 0; i < size; ++i) {
        if (ptr[i] == what) {
            return i;
        }
    }
    return SIZE_MAX;
}

size_t lk_buffer_find(LKBuffer* buf, u8 elem) {
    LK_ASSERT_NOT_NULL(buf);
    return find(buf->data, buf->size, elem);
}

size_t lk_buffer_find_if(LKBuffer* buf, LKBufferPredicate pred) {
    LK_ASSERT_NOT_NULL(buf);
    LK_ASSERT_NOT_NULL(pred);
    for (size_t i = 0; i < buf->size; ++i) {
        if (pred(buf->data[i])) {
            return i;
        }
    }
    return SIZE_MAX;
}

size_t lk_buffer_search(LKBuffer* buf, u8* seq, size_t seq_len) {
    LK_ASSERT_NOT_NULL(buf);
    LK_ASSERT_NOT_NULL(seq);
    if (seq_len > buf->size) {
        return SIZE_MAX;
    }
    size_t pos = 0;
    for (;;) {
        pos += find(&buf->data[pos], buf->size - pos, seq[0]);
        if (pos == SIZE_MAX // not found
            || pos + seq_len > buf->size) { // found but the sequence doesn't fit here anymore
            return SIZE_MAX;
        }
        if (lk_memory_equal(&buf->data[pos], seq, seq_len)) {
            return pos;
        }
        ++pos;
    }
}
