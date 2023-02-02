add_executable(s2-lighting-maps main.cpp)
target_link_libraries(s2-lighting-maps iphelf-opengl iphelf-imgui)
file(COPY shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY textures DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
