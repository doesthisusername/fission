#ifndef FIS_BACKEND_H
#define FIS_BACKEND_H
#include <stdbool.h>
#include "types.h"

#define INIT_WINDOW_WIDTH 600
#define INIT_WINDOW_HEIGHT 900
#define WINDOW_PADDING 10 // need to verify that this is the right thing to do

// only backend we have right now, so...
#if defined(_WIN32) || defined(__linux__)
    #include <glad/gl.h>

    #define NK_INCLUDE_FIXED_TYPES
    #define NK_INCLUDE_STANDARD_IO
    #define NK_INCLUDE_STANDARD_VARARGS
    #define NK_INCLUDE_DEFAULT_ALLOCATOR
    #define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
    #define NK_INCLUDE_FONT_BAKING
    #define NK_KEYSTATE_BASED_INPUT
    #include <nk/nuklear.h>
    #include <nk/nuklear_glfw_gl4.h>

    #define nk_font_stash_begin nk_glfw3_font_stash_begin
    #define nk_font_stash_end nk_glfw3_font_stash_end
#else
    #error unsupported platform, sorry. you can try removing this, but youll be on your own
#endif

struct window_state {
    s32 width;
    s32 height;
    struct nk_vec2 scale;
};
extern struct window_state win_info;

struct render_state {
    size_t row_count;
    float row_height;
    float timer_height_offset; // Timer components can have different row heights
};
extern struct render_state render_state;

/// `ctx` will hold an initialized `nk_context*` on success.
/// Returns true on success.
bool init_nk(struct nk_context** ctx);

/// Shut down GUI libraries, intended for use on application exit.
void shutdown_nk();

/// Returns false if the window has been told to close.
bool still_running();

/// Resize the window to the size specified in the `state`.
/// Intended for use when (re)loading splits and layout.
void resize_window(const struct window_state* state);

/// Should be called at the start of every frame in the main loop.
void start_frame();

/// Should be called at the end of every frame in the main loop.
void end_frame();

#endif // FIS_BACKEND_H
