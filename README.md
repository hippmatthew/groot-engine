# Groot Engine

A custom graphics engine that automates the Vulkan setup process

[documentation](https://hippmatthew.github.io/groot-engine)

### Installation

> Groot Engine uses CMake to generate build files. As such, the following instructions will assume that you are using CMake.

#### Unix Systems

1. clone this repository
2. navigate to the root repository folder
3. make a build directory with `mkdir <directory_name>`
4. navigate to the build directory with `cd <directory_name>`
5. use cmake to generate makefiles with `cmake ..`. Be sure to add any additional options if needed
6. compile Groot Engine with `make`
7. The compiled shared library will be placed in `<build_directory>/src`

To use Groot Engine, make sure to include the headers `-I<groot/engine/headers/path>`, and link to the shared library with `-L<groot/engine/library/path> -lgeng` when compiling your binaries that require it

#### Windows System

> Work in progress...

> There are plans to create a master header file in the future that way you won't have to include the individual files. There are also plans to have an installation script that way your system can find the library and use it as needed