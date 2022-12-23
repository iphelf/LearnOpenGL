add_library(iphelf-opengl
        src/application.cpp
        src/color.cpp
        src/shapearray.cpp
        src/gl/vertices_impl.cpp
        src/program.cpp
        src/gl/program_impl.cpp
        src/shader.cpp)
target_include_directories(iphelf-opengl PUBLIC include)
target_link_libraries(iphelf-opengl
        PRIVATE glfw OpenGL::GL glad)
