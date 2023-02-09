add_executable(s1c03-shaders main.cpp)
target_link_libraries(s1c03-shaders iphelf-opengl)
file(COPY shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
