#include <lk.h>

void* fn(void* arg) {
    printf("hello, %s!\n", (char*)arg);
    return NULL;
}

int main() {
    lk_set_log_file(stdout);
    lk_log("starting thread");
    LKThread thread;
    lk_thread_create(&thread, fn, NULL);
    lk_log("started!");
}
