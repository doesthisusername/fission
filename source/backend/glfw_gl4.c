#define NK_IMPLEMENTATION
#define NK_GLFW_GL4_IMPLEMENTATION
#include "../backend.h"
#undef NK_IMPLEMENTATION
#undef NK_GLFW_GL4_IMPLEMENTATION

#include <stdio.h>
#include "../font.h"

#define MAX_VERTEX_BUFFER (512 * 1024)
#define MAX_ELEMENT_BUFFER (512 * 1024)

static GLFWwindow* window;

static void glfw_error(s32 err, const char* desc) {
    fprintf(stderr, "glfw error %d: %s\n", err, desc);
}

static void resize_callback(GLFWwindow* cb_window, s32 width, s32 height);

bool init_nk(struct nk_context** ctx) {
    // make window
    glfwSetErrorCallback(glfw_error);
    if(!glfwInit()) {
        fprintf(stderr, "glfw failed to init\n");
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    window = glfwCreateWindow(win_info.width, win_info.height, "fission", NULL, NULL);
    glfwMakeContextCurrent(window);

    // make gl
    gladLoaderLoadGL();
    glViewport(0, 0, win_info.width, win_info.height);

    // make ctx
    *ctx = nk_glfw3_init(window, NK_GLFW3_INSTALL_CALLBACKS, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);

    // init win_info more
    s32 width;
    s32 height;
    glfwGetFramebufferSize(window, &width, &height);
    win_info.scale.x = (float)width;
    win_info.scale.y = (float)height;
    reload_fonts = true;

    // callbacks
    glfwSetWindowSizeCallback(window, resize_callback);

    return true;
}

void shutdown_nk() {
    nk_glfw3_shutdown();
    glfwTerminate();
}

bool still_running() {
    return !glfwWindowShouldClose(window);
}

void resize_window(const struct window_state* state) {
    glfwSetWindowSize(window, state->width, state->height);
}

static void resize_callback(GLFWwindow* cb_window, s32 width, s32 height) {
    // resized to the same size
    if(win_info.width == width && win_info.height == height) {
        return;
    }

    win_info.width = width;
    win_info.height = height;

    glfwGetFramebufferSize(cb_window, &width, &height);
    win_info.scale.x = (float)width;
    win_info.scale.y = (float)height;

    render_state.row_height = (win_info.scale.y - WINDOW_PADDING - render_state.timer_height_offset) / render_state.row_count;
    reload_fonts = true;
}

void start_frame() {
    glfwPollEvents();
    nk_glfw3_new_frame();
}

void end_frame() {
    glfwGetWindowSize(window, &win_info.width, &win_info.height);
    glViewport(0, 0, win_info.width, win_info.height);
    glClear(GL_COLOR_BUFFER_BIT);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    nk_glfw3_render(NK_ANTI_ALIASING_ON);
    glfwSwapBuffers(window);
}
