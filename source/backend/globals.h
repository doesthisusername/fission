#ifndef FIS_GLOBALS_H
#define FIS_GLOBALS_H

#include "types.h"

#define INIT_WINDOW_WIDTH 600
#define INIT_WINDOW_HEIGHT 900

// the purpose of this is to "guard" against linker errors as well as save typing on declarations

// basically put a ',' character before the '=' in value initializer, if present
#define STORAGE_INT(decl, ...) decl __VA_ARGS__

// define this in one file (globals.c) before including
#ifdef GLOBALS_C
    #define STORAGE(decl, ...) STORAGE_INT(decl, __VA_ARGS__)
#else
    #define STORAGE(decl, ...) extern STORAGE_INT(decl)
#endif

STORAGE(struct {
    s32 width;
    s32 height;
} win_info, = {
    .width = INIT_WINDOW_WIDTH,
    .height = INIT_WINDOW_HEIGHT
});

#undef STORAGE
#undef STORAGE_INT

#endif // FIS_GLOBALS_H
