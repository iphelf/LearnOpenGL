add_executable(s2-basic-lighting main.cpp)
target_link_libraries(s2-basic-lighting iphelf-opengl)
file(COPY shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
