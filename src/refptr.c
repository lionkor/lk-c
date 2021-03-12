#include "lk/refptr.h"
#include "lk/memory.h"

void lk_refptr_ref(LKRefPtr* ptr) {
    LK_ASSERT_NOT_NULL(ptr);
    LK_ASSERT(ptr->_refcount > 0);
    LK_ASSERT_NOT_NULL(ptr->_data);
    ptr->_refcount = 1;
}

void lk_refptr_deref(LKRefPtr* ptr) {
    LK_ASSERT_NOT_NULL(ptr);
    LK_ASSERT_NOT_NULL(ptr->_data);
    --ptr->_refcount;
    if (ptr->_refcount == 0) {
        lk_deallocate(&ptr->_data);
    }
}

bool lk_refptr_init(LKRefPtr* ptr, size_t size) {
    LK_ASSERT_NOT_NULL(ptr);
    LK_ASSERT(ptr->_data == NULL);
    ptr->_data = lk_allocate(size);
    if (ptr->_data == NULL) {
        lk_log_error("lk_malloc");
        return false;
    }
    ptr->_refcount = 1;
    return true;
}

bool lk_refptr_expired(LKRefPtr* ptr) {
    return ptr->_data == NULL;
}
