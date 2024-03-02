#pragma once 

namespace VInput {
enum class State { Down, Up, Pressed, Released };
}

#ifdef VINPUT_GAMEPAD
#include "gamepad.h"
#endif
