#ifndef FIS_GLFW_GL4_H
#define FIS_GLFW_GL4_H
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_KEYSTATE_BASED_INPUT
#include <nk/nuklear.h>
#include <nk/nuklear_glfw_gl4.h>

#include <stdbool.h>
#include "../types.h"
#include "globals.h"

#define MAX_VERTEX_BUFFER (512 * 1024)
#define MAX_ELEMENT_BUFFER (512 * 1024)

#define nk_font_stash_begin nk_glfw3_font_stash_begin
#define nk_font_stash_end nk_glfw3_font_stash_end

extern GLFWwindow* window;

// ctx: will hold bound nk_context on success
// return: true (i.e. nonzero) on success
bool init_nk(struct nk_context** ctx);

// end nk and glfw (will abort because idk)
void shutdown_nk();

// return false if window closed
bool still_running();

void start_frame();

void end_frame();

#endif // FIS_GLFW_GL4_H