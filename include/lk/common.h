#pragma once
// clang-format off

// WINDOWS
#if defined _WIN32 || defined __CYGWIN__
#define LK_WIN
    #error "implementation for this platform is incomplete"

    #ifdef BUILDING_LK
        #define LK_PUBLIC __declspec(dllexport)
    #else
        #define LK_PUBLIC __declspec(dllimport)
    #endif
// UNIX - assume GNU + POSIX
#else
#define LK_POSIX
    #include <pthread.h>
    #define lk_compat_mutex_type pthread_mutex_t
    #define lk_compat_mutex_init(x) pthread_mutex_init(x, NULL)
    #define lk_compat_mutex_destroy(x) pthread_mutex_destroy(x)
    #define lk_compat_mutex_lock(x) pthread_mutex_lock(x)
    #define lk_compat_mutex_unlock(x) pthread_mutex_unlock(x)
    #define lk_compat_mutex_try_lock(x) pthread_mutex_trylock(x)
    #define lk_compat_condition_type pthread_cond_t
    #define lk_compat_condition_init(cond)                    \
        do {                                           \
            pthread_condattr_t attr;                   \
            pthread_condattr_init(&attr);              \
            pthread_condattr_setpshared(&attr, false); \
            pthread_cond_init(cond, &attr);            \
        } while (false)
    #define lk_compat_condition_destroy(cond) pthread_cond_destroy(cond)
    #define lk_compat_condition_wait(cond, mutex) pthread_cond_wait(cond, mutex)
    #define lk_compat_condition_signal(cond) pthread_cond_signal(cond)

    #ifdef BUILDING_LK
        #define LK_PUBLIC __attribute__((visibility("default")))
    #else
        #define LK_PUBLIC
    #endif
#endif

#if !defined(lk_malloc) && !defined(lk_free) && !defined(lk_calloc)
    #include <stdlib.h>
#endif

#ifndef lk_malloc
    #define lk_malloc malloc
#endif // lk_malloc

#ifndef lk_free
    #define lk_free free
#endif // lk_free

#ifndef lk_calloc
    #define lk_calloc calloc
#endif // lk_calloc

#include <stdio.h>
// clang-format on

void LK_PUBLIC lk_set_log_file(FILE* file);
LK_PUBLIC FILE* lk_get_log_file();

#include <assert.h>
#define LK_ASSERT(x) assert(x)
#define LK_ASSERT_NOT_REACHABLE()           \
    do {                                    \
        lk_log("UNREACHABLE CODE REACHED"); \
        assert(false);                      \
    } while (false)
#define LK_ASSERT_NOT_NULL(x) assert((x) != NULL)
#define lk_log(...)                                        \
    do {                                                   \
        FILE* _lk_logfile = lk_get_log_file();             \
        if (_lk_logfile) {                                 \
            fprintf(_lk_logfile, "[LK] %s: ", __func__); \
            fprintf(_lk_logfile, __VA_ARGS__);             \
            fputs("\n", _lk_logfile);                      \
        }                                                  \
    } while (false)

#include <string.h>
#include <errno.h>
#define lk_log_perror(context) \
    lk_log("%s error: %s", context, strerror(errno))
