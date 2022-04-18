# Projeto EDA
This project uses OpenGL for rendering.

## Requirements
 - C 99+ compiler
 - C++ 8+ compiler
 - CMake >= 3.8
 - Make
 - OpenGL 3.0+

## Compile
### Linux
1 - Crate a build folder and use CMake to build the project with:
```sh
mkdir build
cd build
cmake ..
```
2 - Compile the project using Make:
```sh
make -j 4
```
3 - Run the compiled executable file:
```sh
./ProjetoEDA
```
### Windows
#### Option 1: Visual Studio
Open the project folder in Visual Studio and run.
#### Option 2: Other
Same steps from linux. But after execution `make` a Visual Studio solution will be generated.
