#pragma once 

#include <string>

#include <GLFW/glfw3.h>

#include "input.h"

namespace VInput {

struct GamepadButton {
    State state = State::Released;
    double pressed_at = 0;
    double released_at = 0;
};

struct GamepadAxes {
    float L_X, L_Y, R_X, R_Y, L_Trigger, R_Trigger;

    float& operator[] (size_t idx) { return reinterpret_cast<float*>(this)[idx]; }
    void update(const GLFWgamepadstate& new_state);
};

struct GamepadButtons {
    GamepadButton A, B, X, Y, L_Bumper, R_Bumper, Back, Start, Guide, L_Thumb, R_Thumb, Up, Right, Down, Left;
    GamepadButton &Cross = A, &Circle = B, &Square = X, &Triangle = Y;

    GamepadButton& operator[] (size_t idx) { return reinterpret_cast<GamepadButton*>(this)[idx]; }
    void update(const GLFWgamepadstate& state, const GLFWgamepadstate& new_state);
};

class Gamepad {
    int id;

    GLFWgamepadstate state;

public:
    bool available;
    std::string name;

    GamepadAxes axes;
    GamepadButtons buttons;

    Gamepad();
    ~Gamepad();
    bool init();
    bool update();
};

} // namespace VInput

namespace VInput {
extern Gamepad gamepad;
}

// name: Sony Interactive Entertainment Wireless Controller Touchpad
// 0: pad x (left -> right: -1 -> 1)
// 1: pad y (top -> bottom : -1 -> 1)
//
// name: Sony Interactive Entertainment Wireless Controller Motion Sensors
// 0: rotation about z axis (default: facing up)
// - counterclockwise: (facing up -> virtical -> facing down: 0 -> 0.25 -> 0)
// - clockwise: 0 -> (facing up -> virtical -> facing down: 0 -> -0.25 -> 0)
// 1: rotation about x axis (default: facing user)
// - clockwise: (facing user -> facing up -> facing away from user: 0 -> 0.25 -> 0)
// - counterclockwise: (facing user -> facing down -> facing away from user: 0 -> -0.25 -> 0)
// 2: rotation about x axis (default: facing up)
// 3: accelerometer x
// 4: accelerometer y
// 5: accelerometer z
