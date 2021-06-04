#define NK_IMPLEMENTATION
#define NK_GLFW_GL4_IMPLEMENTATION
#include "glfw_gl4.h"

#include <stdio.h>

GLFWwindow* window;

static void glfw_error(s32 err, const char* desc) {
    fprintf(stderr, "glfw error %d: %s\n", err, desc);
}

bool init(struct nk_context** ctx) {
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
    glViewport(0, 0, win_info.width, win_info.height);
    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK) {
        fprintf(stderr, "glew failed to init\n");
        return false;
    }

    *ctx = nk_glfw3_init(window, NK_GLFW3_INSTALL_CALLBACKS, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
    return true;
}

void shutdown() {
    nk_glfw3_shutdown();
    glfwTerminate();
}
