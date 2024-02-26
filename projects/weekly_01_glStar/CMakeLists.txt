add_executable(main main.cpp)
target_compile_options(main PRIVATE -Wall -Wextra -ggdb)
target_link_libraries(main PRIVATE glad glfw glm::glm)
