cmake_minimum_required(VERSION 3.27.1)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake)
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