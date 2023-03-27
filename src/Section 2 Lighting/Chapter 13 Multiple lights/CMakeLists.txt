add_executable(s2c13-multiple-lights main.cpp lights.h)
target_link_libraries(s2c13-multiple-lights iphelf-opengl iphelf-imgui)
file(COPY shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY textures DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
