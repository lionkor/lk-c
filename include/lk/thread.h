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

// creates a thread, starts it, returns when the thread is running.
bool LK_PUBLIC lk_thread_create(LKThread*, LKThreadFunction fn, void* restrict args);
