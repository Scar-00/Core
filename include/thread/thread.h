#ifndef _CORE_THREAD_H_
#define _CORE_THREAD_H_

#include "../build/build.h"

typedef struct Thread *ThreadHandle;
typedef size_t (*ThreadFunction)(void *args);

CORE_API ThreadHandle thread_spawn(ThreadFunction func, void *args);
CORE_API void thread_join(ThreadHandle self);

#endif // _CORE_THREAD_H_
