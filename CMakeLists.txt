cmake_minimum_required(VERSION 3.20)

project(LearnOpenGL)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
if (MSVC)
    add_compile_options(/W4 /WX)
else ()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif ()

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
add_subdirectory(libs/glad)

add_subdirectory(src)
