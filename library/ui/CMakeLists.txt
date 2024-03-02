add_library(VUI
    ui.cpp
    ui.h
)
target_link_libraries(VUI PRIVATE glfw imgui glad)
target_include_directories(VUI PUBLIC ".")
