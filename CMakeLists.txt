cmake_minimum_required(VERSION 3.27.1)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    if(DEFINED ENV{VCPKG_ROOT})
        set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
            CACHE FILEPATH "vcpkg toolchain file")
    else()
        message(FATAL_ERROR
            "VCPKG_ROOT is not set. Install vcpkg globally and set VCPKG_ROOT to its installation directory, "
            "or configure CMake with -DCMAKE_TOOLCHAIN_FILE=<vcpkg-root>/scripts/buildsystems/vcpkg.cmake.")
    endif()
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(titanforge CXX)

add_subdirectory(packages/common)
add_subdirectory(packages/common_test)
add_subdirectory(packages/graphics)
add_subdirectory(packages/graphics_test)
add_subdirectory(packages/math)
add_subdirectory(packages/math_test)
add_subdirectory(packages/sampleapp)
