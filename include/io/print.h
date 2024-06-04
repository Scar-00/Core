#ifndef _CORE_IO_PRINT_H_
#define _CORE_IO_PRINT_H_

#include "../build/build.h"
#include "../build/defines.h"
#include "file.h"

CORE_API int print(const char *fmt, ...);
CORE_API int println(const char *fmt, ...);
CORE_API int fprint(FileHandle stream, const char *fmt, ...);
CORE_API int fprintln(FileHandle stream, const char *fmt, ...);

#endif // _CORE_IO_PRINT_H_
