#pragma once

#include <GLFW/glfw3.h>
#include <imgui.h>

namespace Helper {

inline double now() { return glfwGetTime(); }

inline double get_frame_time() { return ImGui::GetIO().DeltaTime; }
inline double get_fps() { return ImGui::GetIO().Framerate; }

template <typename T> inline T LERP(T from, T to, float progress) {
    return (1 - progress) * from + progress * to;
}

} // namespace Helper
