#include <lk/lk.h>

int main() {
    lk_set_log_file(stdout);
    LKString str1 = lk_string_create("Hello, World!\n");
    LKString str2 = lk_string_create("Goodbye, World!\n");
    LK_ASSERT(lk_string_writeto(&str1, stdout));
    LK_ASSERT(lk_string_writeto(&str2, stdout));
    lk_string_destroy(&str1);
    lk_string_destroy(&str2);
}
