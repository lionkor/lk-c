#include "chan.h"

#include <errno.h>
#include <string.h>

static FILE* s_logfile = NULL;

#include <assert.h>
#define CHAN_ASSERT(x) assert(x)
#define CHAN_ASSERT_NOT_NULL(x) assert((x) != NULL)
#define chan_log(...)                                    \
    do {                                                 \
        if (s_logfile) {                                 \
            fprintf(s_logfile, "[CHAN] %s: ", __func__); \
            fprintf(s_logfile, __VA_ARGS__);             \
            fputs("\n", s_logfile);                      \
        }                                                \
    } while (false)

bool chan_init(Channel* channel) {
    return chan_init_with_size(channel, 1);
}

bool chan_init_with_size(Channel* channel, size_t size) {
    CHAN_ASSERT_NOT_NULL(channel);
    if (size < 1) {
        chan_log("size needs to be > 0");
        return false;
    }
    channel->_data = CHAN_CALLOC(size, sizeof(ChanValue));
    if (channel->_data == NULL) {
        chan_log("CHAN_CALLOC failed: %s", strerror(errno));
        return false;
    }
    channel->_size = size;
    for (size_t i = 0; i < size; ++i) {
        channel->_data[i].data = NULL;
        channel->_data[i].type = -1;
    }
    channel->_read_ptr = 0;
    channel->_write_ptr = 0;
    // initialize mutex
    CHAN_MUTEX_INIT(&channel->_mutex);
    CHAN_CONDITION_INIT(&channel->_cond_var);
    atomic_store(&channel->_avail, false);
    return true;
}

ChanValue* chan_pop_internal(Channel* channel) {
    // internal, no assert, channel never null
    // we also assume we're already in a mutex locked context
    // and that data is available
    ChanValue* value = &channel->_data[channel->_read_ptr];
    channel->_read_ptr = (channel->_read_ptr + 1) % channel->_size;
    return value;
}

ChanValue chan_pop(Channel* channel) {
    CHAN_ASSERT_NOT_NULL(channel);
    // blocking lock on availability
    // this way we don't need a spinlock to wait on new data,
    // and instead we rely on the mutexes "wait" to block,
    // which is more scheduling friendly.
    // we do NOT unlock this here, ever, as the name says.
    CHAN_MUTEX_LOCK(&channel->_mutex);
    CHAN_ASSERT(CHAN_CONDITION_WAIT(&channel->_cond_var, &channel->_mutex) == 0);
    CHAN_ASSERT(atomic_load(&channel->_avail));
    atomic_store(&channel->_avail, false);
    ChanValue value = *chan_pop_internal(channel);
    CHAN_MUTEX_UNLOCK(&channel->_mutex);
    return value;
}

void chan_push(Channel* channel, void* data, int type) {
    CHAN_ASSERT_NOT_NULL(channel);
    CHAN_MUTEX_LOCK(&channel->_mutex);
    channel->_data[channel->_write_ptr].data = data;
    channel->_data[channel->_write_ptr].type = type;
    channel->_write_ptr = (channel->_write_ptr + 1) % channel->_size;
    atomic_store(&channel->_avail, true);
    CHAN_MUTEX_UNLOCK(&channel->_mutex);
    CHAN_CONDITION_SIGNAL(&channel->_cond_var);
}

bool chan_try_pop(Channel* channel, ChanValue* out_value) {
    CHAN_ASSERT_NOT_NULL(channel);
    CHAN_MUTEX_LOCK(&channel->_mutex);
    bool _ret;
    if (atomic_load(&channel->_avail)) {
        atomic_store(&channel->_avail, false);
        *out_value = *chan_pop_internal(channel);
        _ret = true;
    } else {
        _ret = false;
    }
    CHAN_MUTEX_UNLOCK(&channel->_mutex);
    return _ret;
}

void chan_destroy(Channel* channel) {
    if (channel) {
        CHAN_MUTEX_LOCK(&channel->_mutex);
        free(channel->_data);
        CHAN_MUTEX_UNLOCK(&channel->_mutex);
        CHAN_MUTEX_DESTROY(&channel->_mutex);
        CHAN_CONDITION_DESTROY(&channel->_cond_var);
    }
}

void chan_set_log_file(FILE* file) {
    s_logfile = file;
}

