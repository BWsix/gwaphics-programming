#include <GLFW/glfw3.h>

#include <string>

namespace VInput {

enum class Button { Down, Up, Pressed, Released };

class Gamepad {
    int id;

    GLFWgamepadstate state;

public:
    bool available;
    std::string name;

    float axes[6];
    Button button_state[15];
    double button_pressed_at[15];
    double button_released_at[15];

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
