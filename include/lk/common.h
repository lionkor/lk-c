#pragma once

/*
 * A common set of macros, values, functions and includes shared by all lk
 * headers. You should never need to include this as a user of the library.
 */

#include <stdint.h>

// check for 64 bit integer types, define if exists
#ifdef UINT64_MAX
#define LK_64_BIT
#endif

// clang-format off
// WINDOWS
#if defined _WIN32 || defined __CYGWIN__
#define LK_WIN
    #error "implementation for this platform is incomplete"
    
    #include <windows.h>

    #ifdef BUILDING_LK
        #define LK_PUBLIC __declspec(dllexport)
    #else
        #define LK_PUBLIC __declspec(dllimport)
    #endif
// UNIX - assume GNU + POSIX
#else
#define LK_POSIX
    #ifndef _GNU_SOURCE
        #define _GNU_SOURCE
    #endif // _GNU_SOURCE
    #define __STDC_WANT_LIB_EXT1__ 1
    #include <pthread.h>
    #define lk_compat_mutex_type pthread_mutex_t
    #define lk_compat_mutex_init(x, attr) pthread_mutex_init(x, attr)
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
    #define lk_compat_condition_broadcast(cond) pthread_cond_broadcast(cond)
    #define LK_NORETURN __attribute__ ((__noreturn__))
    #define LK_NODISCARD __attribute__ ((warn_unused_result))

    #ifdef BUILDING_LK
        #define LK_PUBLIC __attribute__((visibility("default")))
    #else
        #define LK_PUBLIC
    #endif
#endif

#include <stdlib.h>
#ifndef lk_malloc
    #define lk_malloc malloc
#endif // lk_malloc

#ifndef lk_free
    #define lk_free free
#endif // lk_free

#ifndef lk_calloc
    #define lk_calloc calloc
#endif // lk_calloc

#ifndef lk_realloc
    #define lk_realloc realloc
#endif // lk_realloc

#include <stdio.h>
// clang-format on

// set to `stdout` to print to stdout, or a logfile FILE* to log to a
// file, or to NULL to turn off logging.
void LK_PUBLIC lk_set_log_file(FILE* file);
LK_PUBLIC FILE* lk_get_log_file();

// don't use
void LK_PUBLIC lk_lock_log_file_mutex();
// don't use
void LK_PUBLIC lk_unlock_log_file_mutex();

#include <assert.h>
#define LK_ASSERT(x) assert(x)
#define LK_ASSERT_NOT_REACHABLE()           \
    do {                                    \
        lk_log("UNREACHABLE CODE REACHED"); \
        assert(false);                      \
    } while (false)
#define LK_ASSERT_NOT_NULL(x) assert((x) != NULL)
#define lk_log(...)                                      \
    do {                                                 \
        FILE* _lk_logfile = lk_get_log_file();           \
        if (_lk_logfile) {                               \
            lk_lock_log_file_mutex();                    \
            fprintf(_lk_logfile, "[LK] %s: ", __func__); \
            fprintf(_lk_logfile, __VA_ARGS__);           \
            fputs("\n", _lk_logfile);                    \
            lk_unlock_log_file_mutex();                  \
        }                                                \
    } while (false)

#include <string.h>
#include <errno.h>
#define lk_log_error(context) \
    lk_log("%s error: %s", context, strerror(errno))

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
#ifdef LK_64_BIT
typedef uint64_t u64;
#endif // LK_64_BIT

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
#ifdef LK_64_BIT
typedef int64_t i64;
#endif // LK_64_BIT

#include <stdbool.h>
#include <stdarg.h>

#define LK_BANNED(func) _##func##_is_banned

#undef strcpy
#define strcpy(x, y) LK_BANNED(strcpy)
#undef strcat
#define strcat(x, y) LK_BANNED(strcat)
#undef strncpy
#define strncpy(x, y, n) LK_BANNED(strncpy)
#undef strncat
#define strncat(x, y, n) LK_BANNED(strncat)
#undef strdup
#define strdup(x) LK_BANNED(strdup)
#undef strndup
#define strndup(x) LK_BANNED(strndup)
#undef scanf
#define scanf(x, ...) LK_BANNED(scanf)
#undef fscanf
#define fscanf(x, ...) LK_BANNED(fscanf)
#undef sscanfs
#define sscanf(x, ...) LK_BANNED(sscanf)
#undef sprintf
#define sprintf(...) LK_BANNED(sprintf)
#undef vsprintf
#define vsprintf(...) LK_BANNED(vsprintf)
#undef vscanf
#define vscanf(str, ap) LK_BANNED(vscanf)
#undef vsscanf
#define vsscanf(str, format, ap) LK_BANNED(vsscanf)
#undef vfscanf
#define sfscanf(str, format, ap) LK_BANNED(vfscanf)
#undef gmtime
#define gmtime(t) LK_BANNED(gmtime)
#undef localtime
#define localtime(t) LK_BANNED(localtime)
#undef ctime
#define ctime(t) LK_BANNED(ctime)
#undef ctime_r
#define ctime_r(t, buf) LK_BANNED(ctime_r)
#undef asctime
#define asctime(t) LK_BANNED(asctime)
#undef asctime_r
#define asctime_r(t, buf) LK_BANNED(asctime_r)
