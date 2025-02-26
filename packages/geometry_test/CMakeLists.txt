project(GeometryTest CXX)

# Dependencies
enable_testing()
find_package(Boost REQUIRED COMPONENTS unit_test_framework)

# Executable files
add_executable(GeometryTest
    src/main.cpp
    src/Matrix3Test.cpp
    src/PolyfaceTest.cpp
    src/Vector2Test.cpp
    src/Vector3Test.cpp
)

# Linked libraries
target_link_libraries(GeometryTest
    PRIVATE Boost::unit_test_framework
    PRIVATE Common::Common
    PRIVATE Geometry::Geometry)

add_test(NAME BoostTests COMMAND boost_tests)