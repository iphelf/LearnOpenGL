add_executable(s1c05-transformations main.cpp)
target_link_libraries(s1c05-transformations iphelf-opengl)
file(COPY shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY textures DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
