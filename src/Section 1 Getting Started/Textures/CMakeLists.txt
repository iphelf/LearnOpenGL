add_executable(s1-textures main.cpp)
target_link_libraries(s1-textures iphelf-opengl)
file(COPY shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY textures DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
