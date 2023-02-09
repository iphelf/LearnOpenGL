add_executable(s2c11-lighting-maps main.cpp)
target_link_libraries(s2c11-lighting-maps iphelf-opengl iphelf-imgui)
file(COPY shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY textures DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
