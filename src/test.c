#include <acutest.h>
#include <lk.h>

void t_chan_init_destroy(void) {
    Channel chan;
    TEST_CHECK(chan_init(&chan) == true);
    chan_destroy(&chan);
}

void t_chan_init_destroy_bad(void) {
    Channel chan;
    TEST_CHECK(chan_init_with_size(&chan, 0) == false);
    TEST_CHECK(chan_init_with_size(&chan, 1) == true);
    chan_destroy(&chan);
    // should be valid
    chan_destroy(NULL);
}

TEST_LIST = {
    { "chan - init, destroy", t_chan_init_destroy },
    { "chan - init, destroy bad", t_chan_init_destroy_bad },
    { NULL, NULL } /* zeroed record marking the end of the list */
};
