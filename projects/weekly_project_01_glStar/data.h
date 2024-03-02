#pragma once

#include <GLFW/glfw3.h>
#include <map>
#include <glm/glm.hpp>

const std::map<int, glm::vec3> COLOR_BUTTON_MAPPING {
    {GLFW_GAMEPAD_BUTTON_CROSS,    {155.0f / 255, 173.0f / 255, 228.0f / 255}},
    {GLFW_GAMEPAD_BUTTON_CIRCLE,   {240.0f / 255, 110.0f / 255, 108.0f / 255}},
    {GLFW_GAMEPAD_BUTTON_SQUARE,   {213.0f / 255, 145.0f / 255, 189.0f / 255}},
    {GLFW_GAMEPAD_BUTTON_TRIANGLE, { 56.0f / 255, 222.0f / 255, 200.0f / 255}},
};

template <typename T> inline T LERP(T from, T to, float progress) {
    return (1 - progress) * from + progress * to;
}

const glm::vec3 STAR_VERTICES[] {
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

