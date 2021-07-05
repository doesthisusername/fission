#ifndef FIS_BACKEND_H
#define FIS_BACKEND_H
#include "types.h"

#define INIT_WINDOW_WIDTH 600
#define INIT_WINDOW_HEIGHT 900

#ifdef _WIN32
    #error win32 not supported yet sorry
#else
    #include "backend/glfw_gl4.h"
#endif

struct window_state {
    s32 width;
    s32 height;
    struct nk_vec2 scale;
};
extern struct window_state win_info;

#endif // FIS_BACKEND_H
