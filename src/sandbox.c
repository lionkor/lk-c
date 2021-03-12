#include <lk/lk.h>

int main() {
    lk_set_log_file(stdout);
    LKBuffer buf = { NULL, 50 };
    buf.data = lk_allocate(buf.size);
    LK_ASSERT(lk_reallocate(&buf.data, 70));
    lk_deallocate(&buf.data);
}
