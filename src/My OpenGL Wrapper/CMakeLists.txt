add_library(iphelf-opengl
        src/application.cpp
        src/color.cpp
        src/program.cpp
        src/shader.cpp
        src/trianglearray.cpp)
target_include_directories(iphelf-opengl PUBLIC include)
target_link_libraries(iphelf-opengl
        PRIVATE glfw OpenGL::GL glad)
