add_library(iphelf-opengl src/window.cpp)
target_include_directories(iphelf-opengl PUBLIC include)
target_link_libraries(iphelf-opengl
        PRIVATE glfw OpenGL::GL glad)