add_executable(s1-shaders main.cpp)
target_link_libraries(s1-shaders iphelf-opengl)
file(COPY shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
