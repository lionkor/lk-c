#include "thread.h"

typedef struct {
    LKThreadFunction original_function;
    void* original_args;
    LKThread* original_thread;
} LKInternalThreadArgs;

bool always_true = true;
void* internal_start_thread(void* args_void_ptr) {
    LKInternalThreadArgs* args = args_void_ptr;
    // pushing to the status channel here signals the caller that the thread has started
    lk_chan_push(&args->original_thread->_status_channel, &always_true, 0);
    lk_chan_push(&args->original_thread->_status_channel, &always_true, 0);
    lk_chan_push(&args->original_thread->_status_channel, &always_true, 0);
    // now we call the original function as usual
    return args->original_function(args->original_args);
}

bool lk_thread_create(LKThread* thread, LKThreadFunction fn, void* restrict args) {
    LK_ASSERT_NOT_NULL(thread);
    // initialize thread
    lk_chan_init(&thread->_status_channel);
    LK_ASSERT_NOT_NULL(fn);
    LKInternalThreadArgs internal_args = {
        .original_function = fn,
        .original_args = args,
        .original_thread = thread
    };
    int ret = lk_compat_thread_create(&thread->_thread, internal_start_thread, &internal_args);
    if (ret != 0) {
        lk_log_perror("thread_create");
        return false;
    }
    LKChanValue val = lk_chan_pop(&thread->_status_channel);
    LK_ASSERT(*((bool*)val.data) == true);
    val = lk_chan_pop(&thread->_status_channel);
    LK_ASSERT(*((bool*)val.data) == true);
    val = lk_chan_pop(&thread->_status_channel);
    LK_ASSERT(*((bool*)val.data) == true);
    return true;
}
