#include "lk.h"

FILE* lk_internal_logfile = NULL;

void lk_set_log_file(FILE* file) {
    lk_internal_logfile = file;
}

FILE* lk_get_log_file() {
    return lk_internal_logfile;
}
