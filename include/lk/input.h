#pragma once

#include "common.h"

void LK_PUBLIC lk_input_init();

#if defined(WIN32)
    HANDLE hConsole_c = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hConsole_c, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hConsole_c, dwMode);
#endif // WIN32
