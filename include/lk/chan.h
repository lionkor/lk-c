#pragma once
// WINDOWS
#if defined _WIN32 || defined __CYGWIN__

#error "mutex implementation is missing for this platform"

#ifdef BUILDING_CHAN
#define CHAN_PUBLIC __declspec(dllexport)
#else
#define CHAN_PUBLIC __declspec(dllimport)
#endif

// UNIX
#else

#include <pthread.h>
#define CHAN_MUTEX_TYPE pthread_mutex_t
#define CHAN_MUTEX_INIT(x) pthread_mutex_init(x, NULL)
#define CHAN_MUTEX_DESTROY(x) pthread_mutex_destroy(x)
#define CHAN_MUTEX_LOCK(x) pthread_mutex_lock(x)
#define CHAN_MUTEX_UNLOCK(x) pthread_mutex_unlock(x)
#define CHAN_MUTEX_TRY_LOCK(x) pthread_mutex_trylock(x)
#define CHAN_CONDITION_TYPE pthread_cond_t
#define CHAN_CONDITION_INIT(cond)                 \
    do {                                          \
        pthread_condattr_t attr;                  \
        pthread_condattr_init(&attr);             \
        pthread_condattr_setpshared(&attr, true); \
        pthread_cond_init(cond, &attr);           \
    } while (false)
#define CHAN_CONDITION_DESTROY(cond) pthread_cond_destroy(cond)
#define CHAN_CONDITION_WAIT(cond, mutex) pthread_cond_wait(cond, mutex)
#define CHAN_CONDITION_SIGNAL(cond) pthread_cond_signal(cond)

#ifdef BUILDING_CHAN
#define CHAN_PUBLIC __attribute__((visibility("default")))
#else
#define CHAN_PUBLIC
#endif
#endif

/*
 * A Channel is a very limited, usually length 1, fully threadsafe queue.
 * Its operation imitates a sort of atomic queue. The usual way to use this is:
 * 
 * thread 1: push(value)
 * thread 2: value = pop();
 * 
 * in which case thread 2 will BLOCK until a value is available. It can be used to
 * send messages between threads, like a shutdown message, or a message to process.
 * 
 * The difference to a standard queue is that it will, once full, override old data.
 * Normal push and pop operations cannot fail and are O(1).
 * 
 * This is inspired by Go's `chan` channels.
 */

#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef CHAN_MALLOC
#define CHAN_MALLOC malloc
#endif // CHAN_MALLOC

#ifndef CHAN_FREE
#define CHAN_FREE free
#endif // CHAN_FREE

#ifndef CHAN_CALLOC
#define CHAN_CALLOC calloc
#endif // CHAN_CALLOC

typedef struct {
    void* data;
    int type;
} ChanValue;

typedef struct {
    ChanValue* _data;
    size_t _size;
    CHAN_MUTEX_TYPE _mutex;
    CHAN_CONDITION_TYPE _cond_var;
    atomic_bool _avail;
    size_t _read_ptr;
    size_t _write_ptr;
} Channel;

// initializes the channel with a queue size of 1; allocates
bool CHAN_PUBLIC chan_init(Channel*);
// initializes the channel with a queue size of `size`; allocates
// requires size > 0
bool CHAN_PUBLIC chan_init_with_size(Channel*, size_t size);
// destroys the channel and all its resources; deallocates
void CHAN_PUBLIC chan_destroy(Channel*);

// pushes the data into the Channel if it has space, replaces old data it if it doesn't
void CHAN_PUBLIC chan_push(Channel*, void* data, int type);
// blocks until a value arrives in the channel, gives it back once it has, and
// removes it from the channel. will NEVER return NULL.
ChanValue CHAN_PUBLIC chan_pop(Channel*);
// non-blocking. pops the value into out_value if its available, returns false if its not
// available
bool chan_try_pop(Channel* channel, ChanValue* out_value);

// statics

// sets a global log file. when set to NULL, will turn off logging (default).
// when set to anything other than NULL, chan_* functions will log to that file.
// it does at no point do any sanity checks on this file, so the caller must ensure that
// the file is open and writable as long as any chan_* functions may be called.
void CHAN_PUBLIC chan_set_log_file(FILE* file);
