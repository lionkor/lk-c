#include "chan.h"

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
    channel->_data = CHAN_CALLOC(size, sizeof(LKChanValue));
    if (channel->_data == NULL) {
        lk_log("CHAN_CALLOC failed: %s", strerror(errno));
        return false;
    }
    channel->_size = size;
    for (size_t i = 0; i < size; ++i) {
        channel->_data[i].data = NULL;
        channel->_data[i].type = -1;
    }
    channel->_read_ptr = 0;
    channel->_write_ptr = 0;
    channel->_count = 0;
    // initialize mutex
    lk_compat_mutex_init(&channel->_mutex);
    lk_compat_condition_init(&channel->_cond_var);
    atomic_store(&channel->_signals, 0);
    return true;
}

LKChanValue chan_pop_internal(LKChannel* channel) {
    // internal, no assert, channel never null
    // we also assume we're already in a mutex locked context
    // and that data is available
    LK_ASSERT(channel->_count > 0);
    LKChanValue value = channel->_data[channel->_read_ptr];
    lk_log("count: %lu", (unsigned long)channel->_count);
    channel->_read_ptr = (channel->_read_ptr + 1) % channel->_size;
    channel->_count -= 1;
    return value;
}

LKChanValue lk_chan_pop(LKChannel* channel) {
    LK_ASSERT_NOT_NULL(channel);
    lk_compat_mutex_lock(&channel->_mutex);
    if (atomic_load(&channel->_signals) == 0) {
        do {
            lk_compat_condition_wait(&channel->_cond_var, &channel->_mutex);
        } while (channel->_count == 0);
    } else {
        lk_log("missed a signal, handling right away!");
    }
    atomic_fetch_sub(&channel->_signals, 1);
    LKChanValue value = chan_pop_internal(channel);
    lk_log("read %p", value.data);
    lk_compat_mutex_unlock(&channel->_mutex);
    return value;
}

void lk_chan_push(LKChannel* channel, void* data, int type) {
    LK_ASSERT_NOT_NULL(channel);
    lk_compat_mutex_lock(&channel->_mutex);
    lk_log("wrote %p", data);
    channel->_data[channel->_write_ptr].data = data;
    channel->_data[channel->_write_ptr].type = type;
    channel->_write_ptr = (channel->_write_ptr + 1) % channel->_size;
    channel->_count += 1;
    atomic_fetch_add(&channel->_signals, 1);
    lk_compat_condition_signal(&channel->_cond_var);
    lk_compat_mutex_unlock(&channel->_mutex);
}

void lk_chan_destroy(LKChannel* channel) {
    if (channel) {
        lk_compat_mutex_lock(&channel->_mutex);
        free(channel->_data);
        lk_compat_mutex_unlock(&channel->_mutex);
        lk_compat_mutex_destroy(&channel->_mutex);
        lk_compat_condition_destroy(&channel->_cond_var);
    }
}
