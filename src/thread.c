#include "thread.h"

#if defined(LK_POSIX)
#include <syscall.h>
#include <unistd.h>
#endif

typedef struct {
    LKThreadFunction original_function;
    void* original_args;
    LKThread* original_thread;
} LKInternalThreadArgs;

bool always_true = true;
void* internal_start_thread(void* args_void_ptr) {
    LKInternalThreadArgs* args = args_void_ptr;
#if defined(LK_POSIX)
    args->original_thread->id = syscall(SYS_gettid);
    lk_log("thread execution started for tid %ld", args->original_thread->id);
#else
#warning "LKThread.id won't be defined"
    lk_log("thread execution started");
#endif
    // pushing to the status channel here signals the caller that the thread has started
    lk_chan_push(&args->original_thread->_status_channel, &always_true);
    // now we call the original function as usual
    void* ret = args->original_function(args->original_args);
    lk_thread_exit(ret);
    LK_ASSERT_NOT_REACHABLE();
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
    thread->id = -1;
    int ret = lk_compat_thread_create(&thread->_thread, internal_start_thread, &internal_args);
    if (ret != 0) {
        lk_log_error("thread_create");
        return false;
    }
    // blocks until the thread reports that it started
    void* val = lk_chan_pop(&thread->_status_channel);
    // if this ever fails, chan is broken
    LK_ASSERT(*((bool*)val) == true);
    return true;
}

bool lk_thread_join(LKThread* thread, void** return_value) {
    LK_ASSERT_NOT_NULL(thread);
    lk_chan_destroy(&thread->_status_channel);
#if !defined(LK_POSIX)
#warning "ensure that the implementation supports return_value of NULL"
#endif
    return (lk_compat_thread_join(thread->_thread, return_value) == 0);
}

void lk_thread_exit(void* value) {
    lk_log("thread execution ended");
    lk_compat_thread_exit(value);
}

bool lk_thread_detach(LKThread* thread) {
    return (lk_compat_thread_detach(thread->_thread) == 0);
}
