add_executable(demo main.cpp)
target_link_libraries(demo PRIVATE glad glfw imgui)

if(UNIX)
    target_compile_options(demo PRIVATE -Wall -Wextra -ggdb)
endif()
