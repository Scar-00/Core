#include "../internal.h"

CORE_API int print(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int ret = vfprintf(stdout, fmt, args);
    va_end(args);
    return ret;
}

CORE_API int println(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int ret = vfprintf(stdout, fmt, args);
    va_end(args);
    printf("\n");
    return ret;
}

CORE_API int fprint(FileHandle stream, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int ret = vfprintf(file_raw(stream), fmt, args);
    va_end(args);
    return ret;
}

CORE_API int fprintln(FileHandle stream, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int ret = vfprintf(file_raw(stream), fmt, args);
    va_end(args);
    fprint(file_raw(stream), "\n");
    return ret;
}
