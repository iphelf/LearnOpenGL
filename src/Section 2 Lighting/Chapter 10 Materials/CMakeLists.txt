add_executable(s2c10-materials main.cpp)
target_link_libraries(s2c10-materials iphelf-opengl iphelf-imgui)
target_include_directories(s2c10-materials PRIVATE ${RAPIDCSV_INCLUDE_DIRS})
file(COPY shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY opengl_vrml_materials.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
