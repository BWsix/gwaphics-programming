add_executable(glStar main.cpp star.hpp helpers.h)
target_link_libraries(glStar PRIVATE glad glfw glm::glm imgui VInput VUI)

if(UNIX)
    target_compile_options(glStar PRIVATE -Wall -Wextra -ggdb)
endif()
