#include <lk.h>

void* write_to_file(LKChannel* chan) {
    FILE* file = fopen("outfile.txt", "w");
    while (true) {
        long c = (long)lk_chan_pop(chan);
        if (c == '!') {
            break;
        }
        fwrite((char*)&c, 1, 1, file);
        fflush(file);
        putchar(c);
        fflush(stdout);
    }
    fclose(file);
    return NULL;
}

int main() {
    LKThread thread;
    LKChannel chars;
    lk_input_init();
    lk_chan_init(&chars);
    lk_thread_create(&thread, (LKThreadFunction)write_to_file, &chars);
    while (true) {
        long c = lk_getchar_no_echo();
        lk_chan_push(&chars, (void*)c);
        if (c == '!') {
            break;
        }
    }
    lk_thread_join(&thread, NULL);
    lk_chan_destroy(&chars);
}