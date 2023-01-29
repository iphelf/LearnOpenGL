add_library(iphelf-imgui
        include/iphelf/imgui/instance.h
        src/instance.cpp)
target_include_directories(iphelf-imgui PUBLIC include)
target_link_libraries(iphelf-imgui PUBLIC imgui::imgui)