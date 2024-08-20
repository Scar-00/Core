#include "../internal.h"
#ifdef PLATFORM_WIN32

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

typedef struct Thread {
    int x;
}Thread;

CORE_API ThreadHandle thread_spawn(ThreadFunction func, void *args) {
    ThreadHandle self = malloc(sizeof(Thread));
    if(!self) {
        return NULL;
    }

    DWORD thread_id;
    CreateThread(NULL, 0, (DWORD(*)(LPVOID))func, args, 0, &thread_id);

    return self;
}

CORE_API void thread_join(ThreadHandle self) {
}

#endif
