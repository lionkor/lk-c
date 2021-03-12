#include <lk/lk.h>

int main() {
    lk_set_log_file(stdout);
    LKBuffer buf = lk_buffer_create(10);
    for (size_t i = 0; i < buf.size; ++i) {
        buf.data[i] = i;
    }
    lk_log("found at: %lu", lk_buffer_find(&buf, 2));
}
