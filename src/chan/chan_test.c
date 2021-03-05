#include <chan.h>
#include <stdio.h>

Channel channel;
Channel thread_started;

void* thread(void* arg) {
    (void)arg;
    puts("starting thread");
    chan_push(&thread_started, NULL, 0);
    while (true) {
        printf("in thread: got %s\n", (char*)chan_pop(&channel).data);
    }
    puts("closing thread");
    return NULL;
}

int main() {
    chan_init_with_size(&channel, 2);
    chan_init(&thread_started);

    pthread_t thid;
    if (pthread_create(&thid, NULL, thread, NULL) != 0) {
        perror("pthread_create() error");
        exit(3);
    }

    (void)chan_pop(&thread_started);

    puts("thread created");
    char* data = "hello";

    chan_push(&channel, data, 0);
    chan_push(&channel, data, 0);
    puts("pushed"); // whats up nerd

    pthread_join(thid, NULL);

    chan_destroy(&channel);
    chan_destroy(&thread_started);
}
