#include <lk.h>

int main() {
    lk_set_log_file(stdout);
    lk_log("started");
    LKRefPtr ptr;
    lk_refptr_init(&ptr, sizeof(int));
    *lk_refptr_as(&ptr, int*) = 5;
    lk_log("data is 0x%x / %d", *lk_refptr_as(&ptr, int*), *lk_refptr_as(&ptr, int*));
    *lk_refptr_as(&ptr, int*) = 10;
    lk_log("data is 0x%x / %d", *lk_refptr_as(&ptr, int*), *lk_refptr_as(&ptr, int*));
    lk_log("expired? %s", lk_refptr_expired(&ptr) ? "true" : "false");
    lk_refptr_deref(&ptr);
    lk_log("expired? %s", lk_refptr_expired(&ptr) ? "true" : "false");
}
