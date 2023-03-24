add_executable(s2c12-light-casters main.cpp lights.h)
target_link_libraries(s2c12-light-casters iphelf-opengl iphelf-imgui)
file(COPY shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY textures DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
