add_library(iphelf-opengl
        include/iphelf/opengl/application.h
        include/iphelf/opengl/camera.h
        include/iphelf/opengl/color.h
        include/iphelf/opengl/input.h
        include/iphelf/opengl/program.h
        include/iphelf/opengl/texture.h
        include/iphelf/opengl/trianglearray.h
        src/application.cpp
        src/camera.cpp
        src/color.cpp
        src/external/gl.cpp
        src/external/gl.h
        src/external/glfw.cpp
        src/external/glfw.h
        src/program.cpp
        src/shader.cpp
        src/shader.h
        src/external/stb_image.cpp
        src/texture.cpp
        src/trianglearray.cpp
        src/window.cpp
        src/window.h)
target_include_directories(iphelf-opengl PUBLIC include)
target_link_libraries(iphelf-opengl
        PRIVATE glfw OpenGL::GL glad::glad glm::glm)
target_include_directories(iphelf-opengl
        PRIVATE ${Stb_INCLUDE_DIR})
