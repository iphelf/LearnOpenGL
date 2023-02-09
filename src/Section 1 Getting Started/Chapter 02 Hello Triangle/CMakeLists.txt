add_executable(s1c02-hello-triangle main.cpp)
target_link_libraries(s1c02-hello-triangle iphelf-opengl)
file(COPY shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
