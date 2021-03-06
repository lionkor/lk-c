#pragma once

#include "chan.h"
#include "common.h"
#include <stdlib.h>

#if defined(LK_WIN)
#error platform not yet implemented
#elif defined(LK_POSIX)
#define lk_compat_thread_type pthread_t
#define lk_compat_thread_create(thread, fn, args) pthread_create(thread, NULL, fn, args)
#define lk_compat_thread_join(thread, retval_ptr_ptr) pthread_join(thread, retval_ptr_ptr)
#else
#error platform not supported
#endif

typedef void* (*LKThreadFunction)(void*);

typedef struct {
    lk_compat_thread_type _thread;
    LKChannel _status_channel;
} LKThread;

// will create a thread, start it, and block until the thread starts to execute.
// this guarantees that after the call to this function the thread is running
// and thus a few nasty issues with condition variables not waiting if there
// are no other threads running are avoided.
bool LK_PUBLIC lk_thread_create(LKThread*, LKThreadFunction fn, void* restrict args);
