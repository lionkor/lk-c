#include "lk/string.h"
#include "lk/memory.h"

LKString lk_string_create(const char* str) {
    LK_ASSERT_NOT_NULL(str);
    size_t len = strlen(str);
    LKString lkstr = {
        .data = lk_allocate(len),
        .len = len,
    };
    LK_ASSERT_NOT_NULL(lkstr.data);
    lk_memory_copy((void*)lkstr.data, str, len);
    return lkstr;
}

void lk_string_destroy(LKString* str) {
    LK_ASSERT_NOT_NULL(str);
    lk_deallocate((void**)&str->data);
    lk_memory_zero(str, sizeof(*str));
}

bool lk_string_writeto(LKString* str, FILE* file) {
    LK_ASSERT_NOT_NULL(str);
    LK_ASSERT_NOT_NULL(file);
    size_t written = fwrite(str->data, 1, str->len, file);
    if (written != str->len) {
        lk_log_error("fwrite");
        return false;
    }
    return true;
}
