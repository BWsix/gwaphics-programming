#include <cassert>

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

#include "data.h"

float fps = 1;
float frame_time = 1;

const char* WINDOW_TITLE = "glStar";
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

const float THICKNESS_OF_STAR = 0.25;
const float MAGNITUDE_OF_OSCILLATION = 0.3;
const float OSCILLATION_PER_SECOND = 1;
const float ROTATION_PER_SECOND = 0.5;

class ColorManager {
    glm::vec3 DEFAULT_COLOR = {0.8, 0.8, 0};
    float progress = 1;
    glm::vec3 prev = DEFAULT_COLOR;
    glm::vec3 current = DEFAULT_COLOR;
    glm::vec3 next = DEFAULT_COLOR;
public:
    glm::vec3 bright(float intensify = 0) const {
        return (1.1f + intensify) * current;
    }
    glm::vec3 dark(float intensify = 0) const {
        return (0.3f + 0.3f * intensify) * current;
    }
    void update() {
        current = LERP(current, next, glm::min<float>(progress, 1));
        progress += 0.01;
    }
    void transform_to(glm::vec3 color) {
        progress = 0;
        prev = current;
        next = color;
    }
    void reset() {
        progress = 1;
        prev = DEFAULT_COLOR;;
        current = DEFAULT_COLOR;;
        next = DEFAULT_COLOR;
    }
} color_manager;

void handle_input(GLFWwindow *window) {
    if (!VInput::gamepad.available) {
        return;
    }

    if (VInput::gamepad.buttons.Back.state == VInput::State::Down) {
        glfwSetWindowShouldClose(window, true);
    }
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix();

    float now = glfwGetTime();

    // setting up transformation
    if (VInput::gamepad.available) {
        float lx = 0.5 * VInput::gamepad.axes.L_X;
        float ly = -0.5 * VInput::gamepad.axes.L_Y;
        float lt = 0.5 * (VInput::gamepad.axes.L_Trigger + 1);
        glTranslatef(lx, ly, lt);

        float rx = VInput::gamepad.axes.R_X;
        float ry = VInput::gamepad.axes.R_Y;
        float theta = 60 * rx;
        float phi = 60 * ry;
        glRotatef(theta, 0, 1, 0);
        glRotatef(phi, 1, 0, 0);
    } else {
        float s = sin(OSCILLATION_PER_SECOND * now);
        float y = MAGNITUDE_OF_OSCILLATION * s * s - MAGNITUDE_OF_OSCILLATION / 2;
        glTranslatef(0, y, 0);

        float r = 360 * now;
        float theta = ROTATION_PER_SECOND * r;
        glRotatef(theta, 0, 1, 0);
    }

    // settings up color
    if (VInput::gamepad.available) {
        for (size_t i = 0; i < 4; i++) {
            if (VInput::gamepad.buttons[i].state == VInput::State::Down) {
                color_manager.transform_to(COLOR_BUTTON_MAPPING.at(i));
            }
        }

        if (VInput::gamepad.buttons.Start.state == VInput::State::Down) {
            color_manager.reset();
        }

        color_manager.update();
    } else {
        color_manager.reset();
    }

    float intensify_color = 0.3 * (VInput::gamepad.axes.R_Trigger + 1) / 2;

    // draw calls
    glBegin(GL_TRIANGLE_FAN);
    {
        glColor3fv(glm::value_ptr(color_manager.bright(intensify_color)));
        glVertex3f(0, 0, THICKNESS_OF_STAR);
        glColor3fv(glm::value_ptr(color_manager.dark(intensify_color)));
        for (auto vertex : STAR_VERTICES) {
            glVertex3fv(glm::value_ptr(vertex));
        }
    }
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    {
        glColor3fv(glm::value_ptr(color_manager.bright(intensify_color)));
        glVertex3f(0, 0, -THICKNESS_OF_STAR);
        glColor3fv(glm::value_ptr(color_manager.dark(intensify_color)));
        for (auto vertex : STAR_VERTICES) {
            glVertex3fv(glm::value_ptr(vertex));
        }
    }
    glEnd();

    glPopMatrix();
}

void update_ui(ImGuiIO& io) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    fps = io.Framerate;
    frame_time = 1000.0f / io.Framerate;

    ImGui::Begin("Info", NULL, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Text("fps: %.0f", fps);
    if (VInput::gamepad.available) {
        ImGui::Text("Gamepad: %s", VInput::gamepad.name.c_str());
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

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void windowSizeCallback(GLFWwindow *, int width, int height) {
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
    assert(glfwInit() && "Failed to initialize GLFW ._.");

    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwSwapInterval(1);

    auto *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
    glfwMakeContextCurrent(window);
    assert(gladLoadGL(glfwGetProcAddress) && "Something went wrong with glad ._.");

    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    windowSizeCallback(window, display_w, display_h);

    glfwSetWindowSizeCallback(window, windowSizeCallback);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    init();
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        VInput::gamepad.update();

        handle_input(window);
        display();
        update_ui(io);

        glfwSwapBuffers(window);
    }

    ImGui_ImplGlfw_Shutdown();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
