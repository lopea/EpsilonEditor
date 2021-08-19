cmake_minimum_required(VERSION 3.19)
project(EpsilonEditor)

# include any libraries
add_subdirectory(lib)

# set the c++ standard
set(CMAKE_CXX_STANDARD 17)

#set any flags
set(CMAKE_CXX_FLAGS "/W3 /WX /EHsc /D_CRT_SECURE_NO_WARNINGS")

#get reference to the opengl library
find_package(OpenGL REQUIRED)

add_executable(EpsilonEditor
        main.cpp
        src/App.cpp
        src/App.h
        src/Window.cpp
        src/Window.h
        src/ImGuiEnvironment.cpp
        src/ImGuiEnvironment.h
        src/ShaderManager.cpp
        src/ShaderManager.h
        src/ImGuiHandler.cpp
        src/ImGuiHandler.h
        src/FileHelper.cpp
        src/FileHelper.h ShaderBuffer.cpp ShaderBuffer.h src/Texture.cpp src/Texture.h)


target_link_libraries(EpsilonEditor OpenGL::GL glad imgui nfd stb)

