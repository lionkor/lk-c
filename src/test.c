#include <acutest.h>
#include <lk.h>

void t_chan_init_destroy() {
    LKChannel chan;
    TEST_CHECK(lk_chan_init(&chan) == true);
    lk_chan_destroy(&chan);
}

void t_chan_init_destroy_bad() {
    LKChannel chan;
    TEST_CHECK(lk_chan_init_with_size(&chan, 0) == false);
    TEST_CHECK(lk_chan_init_with_size(&chan, 1) == true);
    lk_chan_destroy(&chan);
    // should be valid
    lk_chan_destroy(NULL);
}

void* chan_push_thread(void* chan_p) {
    LKChannel* chan = chan_p;
    lk_chan_push(chan, (void*)5ul);
    return NULL;
}

void t_chan_push() {
    LKChannel chan;
    lk_chan_init(&chan);
    LKThread thread;
    lk_thread_create(&thread, chan_push_thread, &chan);
    unsigned long value = (unsigned long)lk_chan_pop(&chan);
    TEST_CHECK(value == 5);
    lk_chan_destroy(&chan);
    lk_thread_join(&thread, NULL);
}

atomic_int n = 0;
void* thread_create_join_thread(void* x) {
    (void)x;
    atomic_fetch_add(&n, 1);
    return &n;
}

void t_thread_create_join() {
    LKThread thread;
    n = 0;
    TEST_CHECK(atomic_load(&n) == 0);
    TEST_CHECK(lk_thread_create(&thread, thread_create_join_thread, NULL));
    TEST_CHECK(thread.id > 0);
    void* retval;
    TEST_CHECK(lk_thread_join(&thread, &retval));
    TEST_CHECK(atomic_load(&n) == 1); // incremented in the thread
    TEST_CHECK(atomic_load(((atomic_int*)retval)) == 1);
    TEST_CHECK(retval == &n);
}

TEST_LIST = {
    { "chan - init, destroy", t_chan_init_destroy },
    { "chan - init, destroy bad", t_chan_init_destroy_bad },
    { "chan - push", t_chan_push },
    { "thread - create, join", t_thread_create_join },
    { NULL, NULL } /* zeroed record marking the end of the list */
};
