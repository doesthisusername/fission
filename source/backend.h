#ifndef FIS_BACKEND_H
#define FIS_BACKEND_H
#include "types.h"

#define INIT_WINDOW_WIDTH 600
#define INIT_WINDOW_HEIGHT 900

#if defined(_WIN32) || defined(__linux__)
    #include "backend/glfw_gl4.h"
#else
    #error unsupported platform sorry
#endif

struct window_state {
    s32 width;
    s32 height;
    struct nk_vec2 scale;
};
extern struct window_state win_info;

#endif // FIS_BACKEND_H
