#include "../internal.h"
#include "build/defines.h"
#include "io/file.h"
#include "string/string.h"
#include <stdio.h>
#include <stdlib.h>


static const String mode_to_string(FileMode_ mode) {
    char mode_buf[64] = "";
    switch (mode) {
        case FILE_READ: {
            snprintf(mode_buf, 64, "%s", "r");
        } break;
        case FILE_APPEND: {
            snprintf(mode_buf, 64, "%s", "a");
        } break;
        case FILE_WRITE: {
            snprintf(mode_buf, 64, "%s", "w");
        } break;
        case FILE_PLUS: {
            snprintf(mode_buf, 64, "%s", "+");
        } break;
        case FILE_BIN: {
            snprintf(mode_buf, 64, "%s", "b");
        } break;
    }
    return string_from(mode_buf);
}

CORE_API FileHandle file_open(const char *path, FileMode_ mode) {
    String mode_str = mode_to_string(mode);
    const char *mode_cstr = string_cstr(&mode_str);
    CORE_ASSERT(mode_str && "invalid file mode");
    FileHandle self = global_allocator.alloc(sizeof(File));
    if(!self) return NULL;
    *self = (File){
        .path = string_from(path),
        .mode = mode,
        .fd = fopen(path, mode_cstr)
    };
    if(!self->fd) {
        return NULL;
    }
    return self;
}

CORE_API void file_close(FileHandle self) {
    fclose(self->fd);
    global_allocator.free(self);
    self = NULL;
}

CORE_API void *file_raw(FileHandle self) {
    return self->fd;
}

CORE_API String file_read(FileHandle self) {
    size_t size = 0;
    fseek(self->fd, 0, SEEK_END);
    size = ftell(self->fd);
    rewind(self->fd);
    char *content = global_allocator.alloc((size + 1) * sizeof(char));
    if(!content)
        return string_new();
    fread(content, sizeof(char), size, self->fd);
    content[size] = '\0';
    String str = string_from_parts(content, size, size);
    global_allocator.free(content);
    return str;
}

CORE_API bool file_write_raw(FileHandle self, const char *data, size_t len) {
    fwrite(data, sizeof(char), len, self->fd);
    return ferror(self->fd) != 0;
}

CORE_API bool file_write(FileHandle self, const StringView data) {
    return file_write_raw(self, data.data, data.len);
}

CORE_API bool file_exists(const StringView path) {
    FILE *fd = fopen(path.data, "r");
    if(fd) {
        fclose(fd);
        return true;
    }
    return false;
}


CORE_API String file_read_to_string(const char *path) {
    FileHandle file = file_open(path, FILE_READ);
#ifndef CORE_DEBUG
    if(!file) {
        fprintf(stderr, "[INFO]: failed to open file `%s`", path);
    }
#endif
    String content = file_read(file);
    file_close(file);
    return content;
}

static bool std_file_handles_init = false;
static File out = {0};
static File err = {0};
static File in = {0};

static void init_file_handles() {
    out = (File) {
        .fd = stdout,
        .mode = FILE_WRITE,
        .path = string_from("STDOUT"),
    };
    err = (File) {
        .fd = stderr,
        .mode = FILE_WRITE,
        .path = string_from("STDERR"),
    };
    in = (File) {
        .fd = stdin,
        .mode = FILE_READ,
        .path = string_from("STDIN"),
    };
    std_file_handles_init = true;
}

CORE_API FileHandle stdio_get(void) {
    if(!std_file_handles_init) {
        init_file_handles();
    }
    return &out;
}

CORE_API FileHandle stderr_get(void) {
    if(!std_file_handles_init) {
        init_file_handles();
    }
    return &err;
}

CORE_API FileHandle stdin_get(void) {
    if(!std_file_handles_init) {
        init_file_handles();
    }
    return &in;
}
