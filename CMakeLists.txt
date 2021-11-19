cmake_minimum_required(VERSION 3.16)

project(Singularity VERSION 0.1)
set(_name ${PROJECT_NAME}-${PROJECT_VERSION})
set(_src
    "src/main.c")
add_executable(${_name} ${_src})
target_link_libraries(${_name} -Wl,-Bstatic -L${PROJECT_SOURCE_DIR}/raylib/build/raylib -lraylib -L${PROJECT_SOURCE_DIR}/raylib/build/raylib/external/glfw/src -lglfw3 -Wl,-Bdynamic -lm -ldl -lpthread)