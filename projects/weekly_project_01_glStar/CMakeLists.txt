add_executable(glStar main.cpp data.h)
target_link_libraries(glStar PRIVATE glad glfw glm::glm imgui VInput)

if(UNIX)
    target_compile_options(glStar PRIVATE -Wall -Wextra -ggdb)
endif()
