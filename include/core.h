#ifndef _CORE_H_
#define _CORE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "build/defines.h"

#include "containers/vector.h"
#include "containers/ranges.h"
#include "containers/array_view.h"

#include "string/string.h"
#include "string/wstring.h"
#include "string/string_view.h"

#include "io/file.h"
#include "io/print.h"

#include "memory/arena/arena.h"
#include "memory/arena/string.h"
#include "memory/arena/vector.h"
#include "memory/alloc.h"

#include "thread/thread.h"

#ifdef CORE_GFX
#include "gfx/gfx.h"
#endif

#ifdef __cplusplus
}
#endif

#endif // _CORE_H_
