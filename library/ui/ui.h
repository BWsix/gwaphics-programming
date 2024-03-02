#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace VUI {

class UI {
    GLFWwindow *window;

public:
    UI(int width, int height, const char* title);
    ~UI();

    void begin_drawing();
    void end_drawing();

    GLFWwindow* get_window() const { return window; };
    bool windowShouldClose() { return glfwWindowShouldClose(window); }
    void closeWindow() { glfwSetWindowShouldClose(window, true); }
};

} // namespace VUI
