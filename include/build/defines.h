#ifndef _CORE_DEFINES_H_
#define _CORE_DEFINES_H_

#ifdef  _WIN32
    #define  PLATFORM_WIN32
#else
    #define PLATFORM_POSIX
#endif

#ifndef STRING_GROW_FACTOR
#define STRING_GROW_FACTOR 1.5
#endif

#ifdef CORE_DEBUG_ASSERT
#define CORE_ASSERT(e) assert(e)
#else
#define CORE_ASSERT(e)
#endif

#include <stdlib.h>

#define CORE_BIT(x) 1 << (x)

#define CORE_CONCAT(a, b) a##b
#define CORE_MACRO_VAR(var) CORE_CONCAT(var, __LINE__)
#define core_defer(begin, end) \
    for(size_t CORE_MACRO_VAR(i) = (begin, 0); !CORE_MACRO_VAR(i); (CORE_MACRO_VAR(i)++), end)

typedef wchar_t wchar;

#endif // _CORE_DEFINES_H_
