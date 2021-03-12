#include "lk/lk.h"

FILE* lk_internal_logfile = NULL;
lk_compat_mutex_type lk_internal_logfile_mutex;

void lk_set_log_file(FILE* file) {
#if defined(LK_POSIX)
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    lk_compat_mutex_init(&lk_internal_logfile_mutex, &attr);
#endif
    lk_internal_logfile = file;
}

FILE* lk_get_log_file() {
    return lk_internal_logfile;
}

bool always_false = false;

void lk_lock_log_file_mutex() {
    //lk_compat_mutex_lock(&lk_internal_logfile_mutex);
}

void lk_unlock_log_file_mutex() {
    //lk_compat_mutex_unlock(&lk_internal_logfile_mutex);
}
