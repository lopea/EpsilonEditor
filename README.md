# Espilon Shader Editor
<b><i> A tiny fragment shader editor </i></b>
<br><img src="./img/Helix_Anim.gif" alt="Helix Gif" width="510" height="334"><img src="./img/Spike_Anim.gif" alt="Spike Gif" width="510" height="334">

## Prerequisites
- C++ 17
- OpenGL 4.6 ( might get changed at a later date )
- CMake 3.19 or higher
- Git
- Windows ( Linux / MacOS at a later date )
- Visual C++

Before compiling this program, you will need to initialize some submodules. 
```bash
$ git submodule init
$ git submodule update --init
```

## Compilation
This project uses CMake as the build automation. the following command will compile Epsilon under the folder `<location>` and with the build type of either `Debug` ,`Release`, or `RelWithDebInfo`.

```bash
$ cmake --build <location> --target EpsilonEditor -DCMAKE_BUILD_TYPE=<Debug|Release|RelWithDebInfo>
```

### Using CLion
You can use Clion with this project by opening the project folder as a CLion project. Clion will take care of the rest. 

### Using Visual Studio
You can use visual studio 2019 with this project by going to File->Open->CMake and select the CMakeLists.txt on the root folder. Once Visual Studio finishes loading the project, click "Select Startup Item" and choose "EpsilonEditor.exe". You can build the program by hitting play.
