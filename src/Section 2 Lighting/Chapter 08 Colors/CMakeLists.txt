add_executable(s2c08-colors main.cpp)
target_link_libraries(s2c08-colors iphelf-opengl)
file(COPY shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
