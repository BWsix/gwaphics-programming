#include <cassert>
#include <cstddef>
#include <cstring>
#include <string>

#include <GLFW/glfw3.h>

#include "gamepad.h"

namespace VInput {
Gamepad gamepad;
}

VInput::Gamepad::Gamepad() {
    assert(glfwInit() && "Failed to initialize GLFW ._.");
    init();
}

VInput::Gamepad::~Gamepad() {
    glfwTerminate();
}

bool VInput::Gamepad::init() {
    for (size_t i = 0; i <= GLFW_JOYSTICK_LAST; i++) {
        if (!glfwJoystickIsGamepad(i)) {
            continue;
        }

        available = true;
        id = i;

        const char *name_cstr = glfwGetJoystickName(i);
        name = name_cstr ? name_cstr : "Unnamed Gamepad";
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
    axes.update(new_state);
    buttons.update(state, new_state);
    state = new_state;

    return true;
}

void VInput::GamepadAxes::update(const GLFWgamepadstate& new_state) {
    for (size_t i = 0; i <= GLFW_GAMEPAD_AXIS_LAST; i++) {
        (*this)[i] = new_state.axes[i];
    }
}

void VInput::GamepadButtons::update(const GLFWgamepadstate& state, const GLFWgamepadstate& new_state) {
    const float now = glfwGetTime();

    for (size_t i = 0; i <= GLFW_GAMEPAD_BUTTON_LAST; i++) {
        const bool button_old = state.buttons[i];
        const bool button_new = new_state.buttons[i];

        if (button_old == false && button_new == true) {
            (*this)[i].state = VInput::State::Down;
            (*this)[i].pressed_at = now;
        } else if (button_old == true && button_new == true) {
            (*this)[i].state = VInput::State::Pressed;
        } else if (button_old == true && button_new == false) {
            (*this)[i].state = VInput::State::Up;
            (*this)[i].released_at = now;
        } else if (button_old == false && button_new == false) {
            (*this)[i].state = VInput::State::Released;
        }
    }
}
