#include <GLFW/glfw3.h>

#include <cassert>
#include <cstddef>
#include <cstring>
#include <string>

#include "gamepad.h"

namespace VInput {
Gamepad gamepad;
}

VInput::Gamepad::Gamepad() {
    init();
}

VInput::Gamepad::~Gamepad() {
    glfwTerminate();
}

bool VInput::Gamepad::init() {
    assert(glfwInit() && "Failed to initialize GLFW ._.");

    for (size_t i = 0; i <= GLFW_JOYSTICK_LAST; i++) {
        if (!glfwJoystickIsGamepad(i)) {
            continue;
        }

        available = true;
        id = i;

        const char *name_cstr = glfwGetJoystickName(i);
        name = "Unnamed Gamepad";
        if (name_cstr) {
            name = name_cstr;
        }

        return true;
    }

    return false;
}

bool VInput::Gamepad::update() {
    if (!available && !init()) {
        return false;
    }
    if (!glfwJoystickIsGamepad(id)) {
        available = false;
        return available;
    }

    const float now = glfwGetTime();

    GLFWgamepadstate new_state;
    glfwGetGamepadState(id, &new_state);

    for (size_t i = 0; i < GLFW_GAMEPAD_BUTTON_LAST; i++) {
        const bool button_old = state.buttons[i];
        const bool button_new = new_state.buttons[i];

        if (button_old == false && button_new == true) {
            button_state[i] = VInput::Button::Down;
            button_pressed_at[i] = now;
        } else if (button_old == true && button_new == true) {
            button_state[i] = VInput::Button::Pressed;
        } else if (button_old == true && button_new == false) {
            button_state[i] = VInput::Button::Up;
            button_pressed_at[i] = now;
        } else if (button_old == false && button_new == false) {
            button_state[i] = VInput::Button::Released;
        }
    }

    for (size_t i = 0; i < GLFW_GAMEPAD_AXIS_LAST; i++) {
        axes[i] = new_state.axes[i];
    }

    state = new_state;

    return true;
}
