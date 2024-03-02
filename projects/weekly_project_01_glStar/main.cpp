#define GLFW_INCLUDE_NONE
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#define VINPUT_GAMEPAD
#include <input.h>
#include <ui.h>

#include "helpers.h"
#include "star.hpp"

const char* WINDOW_TITLE = "glStar";
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

Star star;
using VInput::gamepad;

void update() {
    star.update();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    star.draw();
}

void show_info() {
    ImGui::Begin("Info", NULL, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Text("fps: %.0f", Helper::get_fps());
    ImGui::Text("frametime: %f", Helper::get_frame_time());
    if (gamepad.available) {
        ImGui::Text("Gamepad: %s", gamepad.name.c_str());
        ImGui::Text("Left Stick: movement in xy plane");
        ImGui::Text("Right Stick: rotation");
        ImGui::Text("Left Trigger: movement in z axis");
        ImGui::Text("Right Trigger: intensify the color");
        ImGui::Text("Action Buttons (ones on the right): change color");
        ImGui::Text("Start Button: reset color");
        ImGui::Text("Back Button: close program");
    } else {
        ImGui::Text("No gamepad detected: falled back to animation mode");
    }
    ImGui::End();
}

void window_resize_callback(GLFWwindow *, int width, int height) {
    if (height == 0) {
        height = 1;
    }

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glm::mat4 proj_mat = glm::perspective(glm::radians(45.0f), (float)width / height, 0.1f, 100.0f);
    glMultMatrixf(glm::value_ptr(proj_mat));
    glTranslatef(0, 0, -2);
}

bool init() {
    glShadeModel(GL_SMOOTH);
    glClearColor(0, 0, 0, 0);
    glClearDepth(1);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    return true;
}

int main() {
    auto ui = VUI::UI(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
    window_resize_callback(ui.get_window(), WINDOW_WIDTH, WINDOW_HEIGHT);
    glfwSetWindowSizeCallback(ui.get_window(), window_resize_callback);

    init();

    while (!ui.windowShouldClose()) {
        glfwPollEvents();
        gamepad.update();

        if (gamepad.buttons.Back.state == VInput::State::Down) {
            glfwSetWindowShouldClose(ui.get_window(), true);
        }

        update();
        display();

        ui.begin_drawing();
        {
            show_info();
        }
        ui.end_drawing();

        glfwSwapBuffers(ui.get_window());
    }

    return 0;
}
