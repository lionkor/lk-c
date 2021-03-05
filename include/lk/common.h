#pragma once
// clang-format off

// WINDOWS
#if defined _WIN32 || defined __CYGWIN__
    #error "implementation for this platform is incomplete"

    #ifdef BUILDING_LK
        #define LK_PUBLIC __declspec(dllexport)
    #else
        #define LK_PUBLIC __declspec(dllimport)
    #endif
// UNIX - assume GNU + POSIX
#else
    #include <pthread.h>
    #define lk_mutex_type pthread_mutex_t
    #define lk_mutex_init(x) pthread_mutex_init(x, NULL)
    #define lk_mutex_destroy(x) pthread_mutex_destroy(x)
    #define lk_mutex_lock(x) pthread_mutex_lock(x)
    #define lk_mutex_unlock(x) pthread_mutex_unlock(x)
    #define lk_mutex_try_lock(x) pthread_mutex_trylock(x)
    #define lk_condition_type pthread_cond_t
    #define lk_condition_init(cond)                    \
        do {                                           \
            pthread_condattr_t attr;                   \
            pthread_condattr_init(&attr);              \
            pthread_condattr_setpshared(&attr, false); \
            pthread_cond_init(cond, &attr);            \
        } while (false)
    #define lk_condition_destroy(cond) pthread_cond_destroy(cond)
    #define lk_condition_wait(cond, mutex) pthread_cond_wait(cond, mutex)
    #define lk_condition_signal(cond) pthread_cond_signal(cond)

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
// clang-format on
