add_library(VInput
    gamepad.cpp
    gamepad.h
    input.h
)
target_link_libraries(VInput PRIVATE glfw)
target_include_directories(VInput PUBLIC ".")
