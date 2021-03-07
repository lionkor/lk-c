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
#define lk_compat_thread_exit(return_value) pthread_exit(return_value)
#define lk_compat_thread_detach(thread) pthread_detach(thread)
#else
#error platform not supported
#endif

typedef void* (*LKThreadFunction)(void*);

typedef struct {
    long id;
    lk_compat_thread_type _thread;
    LKChannel _status_channel;
} LKThread;

// Creates a thread, starts it, returns when the thread is running.
// Unlike a normal pthread or similar OS-thread, this guarantees that the thread is valid
// and running after the call.
// After this call LKThread.id is also populated with the thread's id as assigned
// by the operating system.
bool LK_PUBLIC lk_thread_create(LKThread*, LKThreadFunction fn, void* restrict args);
// waits for the thread to terminate
bool LK_PUBLIC lk_thread_join(LKThread*, void** return_value);
void LK_PUBLIC NORETURN lk_thread_exit(void* value);
bool LK_PUBLIC lk_thread_detach(LKThread*);
