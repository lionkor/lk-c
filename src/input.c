#include "input.h"

#if defined(LK_WIN)
static char getchar_no_echo() {
    return _getch();
}
static char getchar_echo() {
    return _getche();
}
#else
#include <termio.h>
// for linux, we take care of non-echoing and non-buffered input with termios
static struct termios s_old_termios, s_current_termios;

static void init_termios(bool echo) {
    tcgetattr(0, &s_old_termios);
    s_current_termios = s_old_termios;
    s_current_termios.c_lflag &= ~ICANON;
    if (echo) {
        s_current_termios.c_lflag |= ECHO;
    } else {
        s_current_termios.c_lflag &= ~ECHO;
    }
    tcsetattr(0, TCSANOW, &s_current_termios);
}

static void reset_termios() {
    tcsetattr(0, TCSANOW, &s_old_termios);
}

static char getch_(bool echo) {
    char ch;
    init_termios(echo);
    ch = getchar();
    reset_termios();
    return ch;
}

static char getchar_no_echo() {
    return getch_(false);
}

static char getchar_echo() {
    return getch_(true);
}
#endif

// input library needs to be initialized before use
bool initialized = false;

void lk_input_init() {
#if defined(LK_WIN)
    HANDLE hConsole_c = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hConsole_c, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hConsole_c, dwMode);
#endif // WIN32
    initialized = true;
}

char lk_unbuffered_getchar_no_echo() {
    LK_ASSERT(initialized);
    return getchar_no_echo();
}

char lk_unbuffered_getchar() {
    LK_ASSERT(initialized);
    return getchar_echo();
}
