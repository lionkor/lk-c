#pragma once

/*
 * Includes all lk library headers.
 * 
 * IMPORTANT: Do *NEVER* use any struct fields starting with _ (underscore) 
 *            directly. This will cause undefined behavior. Fields without a 
 *            underscore prefix may be read or written at any point unless 
 *            documentation says otherwise.
 * 
 * Possible configurations:
 * 
 * - Allocators
 *    lk_malloc, lk_calloc and lk_free can be #define'd before including this
 *    or any lk header to override them with a custom allocator, like so:
 *       #define lk_malloc my_custom_malloc
 *       #define lk_calloc my_custom_calloc
 *       #define lk_free my_custom_free
 *       #define lk_realloc my_custom_realloc
 *    If they're not defined by the user, <stdlib.h>'s malloc, calloc, etc. are
 *    used. Note that you should always define all of them, as undefined
 *    ones will still use the default stdlib version.
 */

#undef LK_VERSION
#define LK_VERSION "0.1"

#include "common.h"

#include "chan.h"
#include "input.h"
#include "refptr.h"
#include "thread.h"
