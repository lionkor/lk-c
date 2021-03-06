#include <lk.h>

void* worker(void* x) {
    LKChannel* chan = x;
    lk_log("waitin");
    lk_log("1 %p", lk_chan_pop(chan));
    sleep(1);
    lk_log("waitin");
    lk_log("2 %p", lk_chan_pop(chan));
    lk_log("waitin");
    lk_log("3 %p", lk_chan_pop(chan));
    return NULL;
}

int main() {
    lk_set_log_file(stdout);
    lk_log("started");
    LKThread thread;
    LKChannel channel;
    lk_chan_init_with_size(&channel, 1);
    lk_chan_push(&channel, (void*)1);
    lk_thread_create(&thread, worker, &channel);
    lk_chan_push(&channel, (void*)2);
    lk_chan_push(&channel, (void*)3);
    sleep(2);
}
