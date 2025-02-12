project(CommonTest CXX)

find_package(Catch2 CONFIG REQUIRED)
include(CTest)
include(Catch)

add_executable(CommonTest
    src/exceptions/IllegalArgumentExceptionTest.cpp
    src/main.cpp
    src/UtilsTest.cpp)

target_link_libraries(CommonTest PRIVATE
    Catch2::Catch2
    Common::Common)

catch_discover_tests(CommonTest)