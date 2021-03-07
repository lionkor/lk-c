#include "refptr.h"

void lk_refptr_ref(LKRefPtr* ptr) {
    LK_ASSERT_NOT_NULL(ptr);
    LK_ASSERT(atomic_load(&ptr->_refcount) > 0);
    LK_ASSERT_NOT_NULL(ptr->_data);
    atomic_fetch_add(&ptr->_refcount, 1);
}

void lk_refptr_deref(LKRefPtr* ptr) {
    LK_ASSERT_NOT_NULL(ptr);
    LK_ASSERT_NOT_NULL(ptr->_data);
    size_t one = 1;
    if (atomic_compare_exchange_strong(&ptr->_refcount, &one, 0)) {
        lk_free(ptr->_data);
        ptr->_data = NULL;
    } else {
        atomic_fetch_sub(&ptr->_refcount, 1);
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
    atomic_store(&ptr->_refcount, 1);
    return true;
}

bool lk_refptr_expired(LKRefPtr* ptr) {
    return ptr->_data == NULL;
}
