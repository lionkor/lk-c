#pragma once

#include "common.h"

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

typedef struct {
    void* data;
    int type;
} LKChanValue;

typedef struct {
    LKChanValue* _data;
    size_t _size;
    lk_compat_mutex_type _mutex;
    lk_compat_condition_type _cond_var;
    size_t _count;
    size_t _read_ptr;
    size_t _write_ptr;
    atomic_size_t _signals;
} LKChannel;

// initializes the channel with a queue size of 1; allocates
bool LK_PUBLIC lk_chan_init(LKChannel*);
// initializes the channel with a queue size of `size`; allocates
// requires size > 0
bool LK_PUBLIC lk_chan_init_with_size(LKChannel*, size_t size);
// destroys the channel and all its resources; deallocates
void LK_PUBLIC lk_chan_destroy(LKChannel*);

// pushes the data into the Channel if it has space, will block if it doesnt until it does
void LK_PUBLIC lk_chan_push(LKChannel*, void* data, int type);
// blocks until a value arrives in the channel, gives it back once it has, and
// removes it from the channel. will NEVER return NULL.
LKChanValue LK_PUBLIC lk_chan_pop(LKChannel*);

// statics

// sets a global log file. when set to NULL, will turn off logging (default).
// when set to anything other than NULL, chan_* functions will log to that file.
// it does at no point do any sanity checks on this file, so the caller must ensure that
// the file is open and writable as long as any chan_* functions may be called.
void LK_PUBLIC lk_chan_set_log_file(FILE* file);
