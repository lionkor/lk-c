#pragma once

/*
 * Includes all lk library headers.
 * 
 * Possible configurations:
 * 
 * - Allocators
 *    lk_malloc, lk_calloc and lk_free can be #define'd before including this
 *    or any lk header to override them with a custom allocator, like so:
 *       #define lk_malloc my_custom_malloc
 *       #define lk_calloc my_custom_calloc
 *       #define lk_free my_custom_free
 *    If they're not defined by the user, <stdlib.h>'s malloc, calloc and free 
 *    are used. If your custom allocator only supplies malloc and free, calloc may
 *    simply be defined as:
 *       #define lk_calloc(x, y) my_custom_malloc((x) * (y)) 
 *    (note the parentheses). Note that you should always define all three, as undefined
 *    ones will still use the default stdlib version.
 */

#undef LK_VERSION
#define LK_VERSION "0.1"

#include "common.h"

#include "chan.h"
#include "refptr.h"
#include "thread.h"
