# TitanForge Development Guide

This document contains instructions on how to build TitanForge on your machine.

<br>

## ☝🏻 First Time Setup

- Ensure that you have a valid C++ compiler installed on your machine.
    - For Windows, [Visual Studio](https://visualstudio.microsoft.com/)
    - For OSX and Linux, [GCC](https://gcc.gnu.org/install/)

- Download [CMake](https://cmake.org/) (version 3.27.1 or above).

- Clone this repository and it's submodules:

    ```
    git clone --recurse-submodules https://github.com/NateRex/titanforge.git
    ```

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

After compiling, all libraries and executables generated will be located within a `bin` folder within the that module's build directory. To run the executable, execute a command like so:

```
./.build/packages/<module>/bin/<executable>.exe
```

This method works for both unit test modules, as well as applications such as `sampleapp`.