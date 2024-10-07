#ifndef _CORE_BUILD_H_
#define _CORE_BUILD_H_

#if defined(DLL_BUILD) && defined (win32)
    #define CORE_API  __declspec(dllexport)
#elif defined(DLL_USE)
    #define CORE_API __declspec(dllimport)
#else
    #define CORE_API
#endif

#include <stdbool.h>

#endif // _CORE_BUILD_H_
