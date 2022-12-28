add_executable(s1-hello-triangle main.cpp)
target_link_libraries(s1-hello-triangle iphelf-opengl)
file(COPY shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
