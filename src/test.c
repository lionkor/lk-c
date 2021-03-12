#include <acutest.h>
#include <lk/lk.h>

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

_Atomic(int) n = 0;
void* thread_create_join_thread(void* x) {
    (void)x;
    ++n;
    return &n;
}

void t_thread_create_join() {
    LKThread thread;
    n = 0;
    TEST_CHECK(n == 0);
    TEST_CHECK(lk_thread_create(&thread, thread_create_join_thread, NULL));
    TEST_CHECK(thread.id > 0);
    void* retval;
    TEST_CHECK(lk_thread_join(&thread, &retval));
    TEST_CHECK(n == 1); // incremented in the thread
    TEST_CHECK(*((_Atomic(int)*)retval) == 1);
    TEST_CHECK(retval == &n);
}

void t_memory_alloc_dealloc() {
    void* mem = lk_allocate(1);
    TEST_ASSERT(mem != NULL);
    lk_deallocate(&mem);
    TEST_CHECK(mem == NULL);
}

void t_memory_copy() {
    const size_t n = 64;
    void* mem = lk_allocate(n);
    TEST_ASSERT(mem != NULL);

    lk_memory_zero(mem, n);
    const u8* p = mem;
    for (size_t i = 0; i < n; ++i) {
        TEST_CHECK(p[i] == 0);
    }

    lk_memory_fill(mem, n, (u8)1);
    p = mem;
    for (size_t i = 0; i < n; ++i) {
        TEST_CHECK(p[i] == (u8)1);
    }

    lk_memory_fill(mem, n, (u8)-253);
    p = mem;
    for (size_t i = 0; i < n; ++i) {
        TEST_CHECK(p[i] == (u8)-253);
    }

    void* mem2 = lk_allocate(n);
    lk_memory_zero(mem2, n);
    lk_memory_copy(mem2, mem, n);
    p = mem;
    for (size_t i = 0; i < n; ++i) {
        TEST_CHECK(p[i] == (u8)-253);
    }
    p = mem2;
    for (size_t i = 0; i < n; ++i) {
        TEST_CHECK(p[i] == (u8)-253);
    }

    lk_memory_fill(mem, n, 123);
    lk_memory_move(mem2, mem, n);

    p = mem2;
    for (size_t i = 0; i < n; ++i) {
        TEST_CHECK(p[i] == (u8)-253);
    }

    lk_deallocate(&mem);
    lk_deallocate(&mem2);
    TEST_CHECK(mem == NULL);
    TEST_CHECK(mem2 == NULL);
}

void t_buffer_alloc_dealloc() {
    LKBuffer buf = lk_buffer_create(10);
    lk_buffer_destroy(&buf);
    TEST_CHECK(buf.data == NULL);
    TEST_CHECK(buf.size == 0);
}

static bool is_X(u8 c) {
    return c == 'X';
}

void t_buffer_find() {
    lk_set_log_file(stdout);
    LKBuffer buf = lk_buffer_create(10);
    TEST_CHECK(lk_buffer_find(&buf, 1) == SIZE_MAX);
    TEST_CHECK(lk_buffer_find(&buf, 0) == 0);
    buf.data[2] = 'a';
    buf.data[3] = 'a';
    buf.data[4] = 'X';
    TEST_CHECK(lk_buffer_find(&buf, 'a') == 2);
    TEST_CHECK(lk_buffer_find_if(&buf, is_X) == 4);
    u8 tofind[] = { 'a', 'X' };
    TEST_CHECK(lk_buffer_search(&buf, tofind, 2) == 3);
    lk_buffer_zero(&buf);
    TEST_CHECK(lk_buffer_search(&buf, tofind, 2) == SIZE_MAX);
    lk_buffer_destroy(&buf);
    TEST_CHECK(buf.data == NULL);
    TEST_CHECK(buf.size == 0);
}

TEST_LIST = {
    { "chan - init, destroy", t_chan_init_destroy },
    { "chan - init, destroy bad", t_chan_init_destroy_bad },
    { "chan - push", t_chan_push },
    { "thread - create, join", t_thread_create_join },
    { "memory - allocate, deallocate", t_memory_alloc_dealloc },
    { "buffer - allocate, deallocate", t_buffer_alloc_dealloc },
    { "buffer - find", t_buffer_find },
    { NULL, NULL } /* zeroed record marking the end of the list */
};
