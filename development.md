# TitanForge Development Guide

This document contains instructions on how to build TitanForge on your machine.

<br>

## ☝🏻 First Time Setup

- Ensure that you have a valid C++ compiler installed on your machine.
    - **Windows** - [Visual Studio](https://visualstudio.microsoft.com/).
    - **OSX and Linux** - [GCC](https://gcc.gnu.org/install/)

- Download [CMake](https://cmake.org/) (version 3.27.1 or above).

- Clone this repository and it's submodules:

    ```
    git clone --recurse-submodules https://gitlab.com/ir0nsoftware/core.git
    ```

- Install the dependencies of this project:
    
    ```
    cd vcpkg
    ./bootstrap-vcpkg
    vcpkg install catch2:x64-windows
    ```

    - Replace `x64-windows` if using a different operating system.

<br>

## 🛠️ Compiling

- Initialize the build

    ```
    cd .build
    cmake ..
    ```

- Compile

    ```
    cd .build
    cmake --build .
    ```

<br>

## 🚀 Running

After compiling, if a module produces an executable file, it will be located in a `Debug` folder within the that module's build directory. To run the executable, execute a command like so:

```
.\.build\<module>\Debug\<executable>.exe
```

This method works for both unit test modules, as well as applications such as `sampleapp`.