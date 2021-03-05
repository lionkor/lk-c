#include <lthread.h>

#include <assert.h>
#define LTHREAD_ASSERT(x) assert(x)
#define LTHREAD_ASSERT_NOT_NULL(x) assert((x) != NULL)

bool lthread_init(LThread* thread, LThreadFunction fn) {
    
}
