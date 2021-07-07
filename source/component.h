#ifndef FIS_COMPONENT_H
#define FIS_COMPONENT_H
#include <glad/gl.h>

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_KEYSTATE_BASED_INPUT
#define NK_GLFW_MAX_TEXTURES 2048
#include <nk/nuklear.h>
#include <nk/nuklear_glfw_gl4.h>

#include <livesplit_core.h>

void draw(struct nk_context* ctx, LayoutStateRef state, size_t index);

void draw_timer(struct nk_context* ctx, TimerComponentStateRef state);
void draw_title(struct nk_context* ctx, TitleComponentStateRef state);

#endif // FIS_COMPONENT_H
