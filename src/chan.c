#include "lk/chan.h"

#include "lk/memory.h"

#include <errno.h>
#include <string.h>

bool lk_chan_init(LKChannel* channel) {
    return lk_chan_init_with_size(channel, 1);
}

bool lk_chan_init_with_size(LKChannel* channel, size_t size) {
    LK_ASSERT_NOT_NULL(channel);
    if (size < 1) {
        lk_log("size needs to be > 0");
        return false;
    }
    channel->_data = lk_allocate_array(size, sizeof(LKChanValue));
    if (channel->_data == NULL) {
        lk_log_error("lk_calloc");
        return false;
    }
    channel->_size = size;
    for (size_t i = 0; i < size; ++i) {
        channel->_data[i].data = NULL;
    }
    channel->_read_ptr = 0;
    channel->_write_ptr = 0;
    channel->_count = 0;
    int ret;
#if defined(LK_POSIX)
    pthread_mutexattr_t attr;

    ret = pthread_mutexattr_init(&attr);
    if (ret != 0) {
        lk_log_error("mutexattr_init");
    }
    ret = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    if (ret != 0) {
        lk_log_error("mutexattr_settype");
    }
    ret = pthread_mutex_init(&channel->_mutex, &attr);
    if (ret != 0) {
        lk_log_error("mutex_init");
    }
#endif
    lk_compat_condition_init(&channel->_cond_var);
    lk_compat_condition_init(&channel->_free_cond_var);
    return true;
}

LKChanValue chan_pop_internal(LKChannel* channel) {
    // internal, no assert, channel never null
    // we also assume we're already in a mutex locked context
    // and that data is available
    LK_ASSERT(channel->_count > 0);
    LKChanValue value = channel->_data[channel->_read_ptr];
    channel->_read_ptr = (channel->_read_ptr + 1) % channel->_size;
    channel->_count -= 1;
    return value;
}

void* lk_chan_pop(LKChannel* channel) {
    LK_ASSERT_NOT_NULL(channel);
    int ret = lk_compat_mutex_lock(&channel->_mutex);
    if (ret != 0) {
        lk_log_error("mutex_lock");
    }
    while (channel->_count == 0) {
        ret = lk_compat_condition_wait(&channel->_cond_var, &channel->_mutex);
        if (ret != 0) {
            lk_log_error("condition_wait");
        }
        lk_log("waiting");
    }
    LKChanValue value = chan_pop_internal(channel);
    ret = lk_compat_condition_signal(&channel->_free_cond_var);
    if (ret != 0) {
        lk_log_error("condition_signal");
    }
    ret = lk_compat_mutex_unlock(&channel->_mutex);
    if (ret != 0) {
        lk_log_error("mutex_unlock");
    }
    return value.data;
}

void lk_chan_push(LKChannel* channel, void* data) {
    LK_ASSERT_NOT_NULL(channel);
    int ret = lk_compat_mutex_lock(&channel->_mutex);
    if (ret != 0) {
        lk_log_error("mutex_lock");
    }
    while (channel->_count >= channel->_size) {
        ret = lk_compat_condition_wait(&channel->_free_cond_var, &channel->_mutex);
        if (ret != 0) {
            lk_log_error("condition_wait");
        }
    }
    channel->_data[channel->_write_ptr].data = data;
    channel->_write_ptr = (channel->_write_ptr + 1) % channel->_size;
    channel->_count += 1;
    ret = lk_compat_condition_signal(&channel->_cond_var);
    if (ret != 0) {
        lk_log_error("condition_signal");
    }
    ret = lk_compat_mutex_unlock(&channel->_mutex);
    if (ret != 0) {
        lk_log_error("mutex_unlock");
    }
}

void lk_chan_destroy(LKChannel* channel) {
    if (channel) {
        lk_compat_mutex_lock(&channel->_mutex);
        lk_deallocate((void**)&channel->_data);
        lk_compat_mutex_unlock(&channel->_mutex);
        lk_compat_mutex_destroy(&channel->_mutex);
        lk_compat_condition_destroy(&channel->_cond_var);
    }
}
