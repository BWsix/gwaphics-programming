#pragma once

#include <vector>
#include <map>

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
#include "helpers.h"

class Transform {
public:
    glm::mat4 model {1};

    void load_identity() {
        model = {1};
    }

    glm::vec3 apply(glm::vec3 vertex) {
        return glm::vec3(model * glm::vec4(vertex, 1));
    }

    void translate(glm::vec3 translation) {
        model = glm::translate(model, translation);
    }

    // angle: in radian
    void rotate(float angle, glm::vec3 axis) {
        model = glm::rotate(model, angle, axis);
    }
};

class Star {
    Transform transform;
    float intensify_color;

    const float THICKNESS_OF_STAR = 0.25;
    const float MAGNITUDE_OF_OSCILLATION = 0.3;
    const float OSCILLATION_PER_SECOND = 1;
    const float ROTATION_PER_SECOND = 0.5;

    const std::vector<glm::vec3> STAR_VERTICES {
        { 0.0,  0.7, 0.0}, // top
        {-0.2,  0.4, 0.0}, // top > left
        {-0.6,  0.4, 0.0}, // left
        {-0.3,  0.0, 0.0}, // left > bottom-left
        {-0.5, -0.4, 0.0}, // bottom-left
        { 0.0, -0.2, 0.0}, // bottom-left > bottom-right
        { 0.5, -0.4, 0.0}, // bottom-right
        { 0.3,  0.0, 0.0}, // bottom-right > right
        { 0.6,  0.4, 0.0}, // right
        { 0.2,  0.4, 0.0}, // right > top
        { 0.0,  0.7, 0.0}, // top
    };

public:
    void update() {
        transform.load_identity();

        // handle transformation
        if (VInput::gamepad.available) {
            float lx = 0.5 * VInput::gamepad.axes.L_X;
            float ly = -0.5 * VInput::gamepad.axes.L_Y;
            float lt = 0.5 * (VInput::gamepad.axes.L_Trigger + 1);
            transform.translate({ lx, ly, lt });

            float rx = VInput::gamepad.axes.R_X;
            float ry = VInput::gamepad.axes.R_Y;
            float theta = 60 * rx;
            float phi = 60 * ry;
            transform.rotate(glm::radians(theta), {0, 1, 0});
            transform.rotate(glm::radians(phi), {1, 0, 0});
        } else {
            float s = sin(OSCILLATION_PER_SECOND * Helper::now());
            float y = MAGNITUDE_OF_OSCILLATION * s * s - MAGNITUDE_OF_OSCILLATION / 2;
            transform.translate({ 0, y, 0 });

            float r = 360 * Helper::now();
            float theta = ROTATION_PER_SECOND * r;
            transform.rotate(glm::radians(theta), {0, 1, 0});
        }

        // handle color
        if (VInput::gamepad.available) {
            for (size_t i = 0; i < 4; i++) {
                if (VInput::gamepad.buttons[i].state == VInput::State::Down) {
                    color.transform_to(color.get_color_from_button(i));
                }
            }

            if (VInput::gamepad.buttons.Start.state == VInput::State::Down) {
                color.reset();
            }

            color.update();
        } else {
            color.reset();
        }

        intensify_color = 0.3 * (VInput::gamepad.axes.R_Trigger + 1) / 2;
    }

    void draw() {
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glPushMatrix();

        glBegin(GL_TRIANGLE_FAN);
        {
            glColor3fv(glm::value_ptr(color.bright(intensify_color)));
            glVertex3fv(glm::value_ptr(transform.apply({0, 0, THICKNESS_OF_STAR})));
            glColor3fv(glm::value_ptr(color.dark(intensify_color)));
            for (auto vertex : STAR_VERTICES) {
                glVertex3fv(glm::value_ptr(transform.apply(vertex)));
            }
        }
        glEnd();

        glBegin(GL_TRIANGLE_FAN);
        {
            glColor3fv(glm::value_ptr(color.bright(intensify_color)));
            glVertex3fv(glm::value_ptr(transform.apply({0, 0, -THICKNESS_OF_STAR})));
            glColor3fv(glm::value_ptr(color.dark(intensify_color)));
            for (auto vertex : STAR_VERTICES) {
                glVertex3fv(glm::value_ptr(transform.apply(vertex)));
            }
        }
        glEnd();

        glPopMatrix();
    }

    class Color {
        const float COLOR_TRANSITION_DURATION = 0.5;

        const glm::vec3 DEFAULT_COLOR = {0.8, 0.8, 0};
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
            current = Helper::LERP(prev, next, glm::min<float>(progress, 1));
            progress += 1 / COLOR_TRANSITION_DURATION * Helper::get_frame_time();
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
        static glm::vec3 get_color_from_button(int id) {
            const std::map<int, glm::vec3> COLOR_BUTTON_MAPPING {
                {GLFW_GAMEPAD_BUTTON_CROSS,    {155.0f / 255, 173.0f / 255, 228.0f / 255}},
                {GLFW_GAMEPAD_BUTTON_CIRCLE,   {240.0f / 255, 110.0f / 255, 108.0f / 255}},
                {GLFW_GAMEPAD_BUTTON_SQUARE,   {213.0f / 255, 145.0f / 255, 189.0f / 255}},
                {GLFW_GAMEPAD_BUTTON_TRIANGLE, { 56.0f / 255, 222.0f / 255, 200.0f / 255}},
            };
            return COLOR_BUTTON_MAPPING.at(id);
        }
    } color;
};
