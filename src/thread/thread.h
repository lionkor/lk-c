#pragma once

#include <chan/chan.h>
#include <stdlib.h>

typedef void* (*LThreadFunction)(void*);

typedef struct {
    //LTHREAD_THREAD_TYPE _thread;
    LThreadFunction _fn;
} LThread;

//bool LTHREAD_PUBLIC lthread_create(LThread*, LThreadFunction fn);
