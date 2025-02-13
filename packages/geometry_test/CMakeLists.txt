project(GeometryTest CXX)

find_package(Catch2 CONFIG REQUIRED)
include(CTest)
include(Catch)

add_executable(GeometryTest
    src/main.cpp
    src/Matrix3Test.cpp
    src/Vector2Test.cpp
    src/Vector3Test.cpp)

target_link_libraries(GeometryTest
    PRIVATE Catch2::Catch2
    PRIVATE Common::Common
    PRIVATE Geometry::Geometry)

catch_discover_tests(GeometryTest)