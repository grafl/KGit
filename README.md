# KGit

## Required things
- C++ tools
- CMake
- libgit2

## C++ tools
You must have an already installed C++ compiler and linker (GNU gcc or Visual Studio).

## CMake
You must download the official distribution from https://github.com/Kitware/CMake/releases/download/v3.29.0-rc2/cmake-3.29.0-rc2-windows-x86_64.zip
and unzip and place it in a folder what is in your PATH.

## libgit2
First, download it from https://github.com/libgit2/libgit2/archive/refs/tags/v1.7.2.zip 
and unzip it in a temp folder. Just following the official [documentation](https://libgit2.org/docs/guides/build-and-link/) 
you can build it with CMake.

After build, in the same folder, run the `cmake --install .` command what should install the library somewhere 
under your `C:/Program Files (x86)/libgit2`.

## KGit

Once you have the required components, you can (could) build this POC.

```shell
cd C:\src
git clone KGit
cd KGit
mkdir build
cd build
cmake -S .. -B .
cmake --build .
copy "C:\Program Files (x86)\libgit2\bin\git2.dll" .
Debug\KGit.exe
```
