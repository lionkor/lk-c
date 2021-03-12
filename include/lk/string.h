#pragma once

#include "lk/common.h"

// an immutable-length string
typedef struct {
    const size_t len;
    const char* data;
} LKString;

// creates a string and returns it.
LK_PUBLIC LKString lk_string_create(const char* str);
// the only function which will ever modify the string,
// as it will free memory and zero it out.
LK_PUBLIC void lk_string_destroy(LKString* str);
// writes the string to a file.
LK_PUBLIC LK_NODISCARD bool lk_string_writeto(LKString* str, FILE* file);
