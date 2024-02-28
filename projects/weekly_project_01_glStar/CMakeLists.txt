add_executable(glStar main.cpp)
target_link_libraries(glStar PRIVATE glad glfw glm::glm)

if(UNIX)
    target_compile_options(glStar PRIVATE -Wall -Wextra -ggdb)
endif()
