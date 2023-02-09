add_executable(s2c09-basic-lighting main.cpp)
target_link_libraries(s2c09-basic-lighting iphelf-opengl iphelf-imgui)
file(COPY shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
