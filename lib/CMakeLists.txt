#add the glad library
add_subdirectory(glad)

#add files for the imgui library

add_library(imgui INTERFACE)

target_sources(imgui INTERFACE imgui/imgui.cpp
        imgui/imgui_draw.cpp
        imgui/imgui_demo.cpp
        imgui/imgui_tables.cpp
        imgui/imgui_widgets.cpp
        imgui/backends/imgui_impl_win32.cpp
        imgui/backends/imgui_impl_opengl3.cpp
        ImguiText/TextEditor.cpp)

target_include_directories(imgui INTERFACE imgui ImguiText)

# add the nfd library
add_library(nfd INTERFACE
        nfd/src/nfd_common.c
        nfd/src/nfd_win.cpp
        )
target_include_directories(nfd INTERFACE nfd/include)