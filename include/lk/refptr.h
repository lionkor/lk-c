#pragma once

#include "common.h"

typedef struct {
    void* _data;
    atomic_size_t _refcount;
} LKRefPtr;

bool LK_PUBLIC lk_refptr_init(LKRefPtr*, size_t size);

#define lk_refptr_as(ptr, type) \
    ((type)((ptr)->_data))
    
bool LK_PUBLIC lk_refptr_expired(LKRefPtr*);
void LK_PUBLIC lk_refptr_ref(LKRefPtr* ptr);
void LK_PUBLIC lk_refptr_deref(LKRefPtr* ptr);
