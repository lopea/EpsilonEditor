cmake_minimum_required(VERSION 3.19)
project(EpsilonEditor)

# include any libraries
add_subdirectory(lib)

# set the c++ standard
set(CMAKE_CXX_STANDARD 17)

#set any flags
set(CMAKE_CXX_FLAGS "/W3 /WX /EHsc")

find_package(OpenGL)

add_executable(EpsilonEditor main.cpp src/App.cpp src/App.h src/Window.cpp src/Window.h src/ImGuiEnvironment.cpp src/ImGuiEnvironment.h ShaderManager.cpp ShaderManager.h)

target_link_libraries(EpsilonEditor OpenGL::GL glad imgui)

