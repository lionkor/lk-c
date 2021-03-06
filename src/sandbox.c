#include <lk.h>

void* fn(void* arg) {
    (void)arg;
    lk_log("hello!");
    return NULL;
}

int main() {
    LKThread thread;
    lk_thread_create(&thread, fn, NULL);
}
