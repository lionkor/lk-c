#include "refptr.h"

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
        lk_free(ptr->_data);
        ptr->_data = NULL;
    }
}

bool lk_refptr_init(LKRefPtr* ptr, size_t size) {
    LK_ASSERT_NOT_NULL(ptr);
    LK_ASSERT(ptr->_data == NULL);
    ptr->_data = lk_malloc(size);
    if (ptr->_data == NULL) {
        lk_log_perror("lk_malloc");
        return false;
    }
    ptr->_refcount = 1;
    return true;
}

bool lk_refptr_expired(LKRefPtr* ptr) {
    return ptr->_data == NULL;
}
