#include <acutest.h>
#include <lk.h>

void t_chan_init_destroy(void) {
    LKChannel chan;
    TEST_CHECK(lk_chan_init(&chan) == true);
    lk_chan_destroy(&chan);
}

void t_chan_init_destroy_bad(void) {
    LKChannel chan;
    TEST_CHECK(lk_chan_init_with_size(&chan, 0) == false);
    TEST_CHECK(lk_chan_init_with_size(&chan, 1) == true);
    lk_chan_destroy(&chan);
    // should be valid
    lk_chan_destroy(NULL);
}

TEST_LIST = {
    { "chan - init, destroy", t_chan_init_destroy },
    { "chan - init, destroy bad", t_chan_init_destroy_bad },
    { NULL, NULL } /* zeroed record marking the end of the list */
};
