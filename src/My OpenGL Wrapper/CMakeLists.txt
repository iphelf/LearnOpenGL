add_library(iphelf-opengl
        include/iphelf/opengl/application.h
        include/iphelf/opengl/color.h
        include/iphelf/opengl/input.h
        include/iphelf/opengl/program.h
        include/iphelf/opengl/texture.h
        include/iphelf/opengl/trianglearray.h
        src/application.cpp
        src/color.cpp
        src/gl.cpp
        src/gl.h
        src/glfw.cpp
        src/glfw.h
        src/program.cpp
        src/shader.cpp
        src/shader.h
        src/texture.cpp
        src/trianglearray.cpp
        src/window.cpp
        src/window.h)
target_include_directories(iphelf-opengl PUBLIC include)
target_link_libraries(iphelf-opengl
        PRIVATE glfw OpenGL::GL glad stb_image glm::glm)
