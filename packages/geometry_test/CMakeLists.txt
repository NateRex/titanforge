project(GeometryTest CXX)

enable_testing()
find_package(Boost REQUIRED COMPONENTS unit_test_framework)

add_executable(GeometryTest
    src/main.cpp
    src/Matrix3Test.cpp
    src/PolyfaceTest.cpp
    src/Vector2Test.cpp
    # src/Vector3Test.cpp)
)

target_link_libraries(GeometryTest
    PRIVATE Boost::unit_test_framework
    PRIVATE Common::Common
    PRIVATE Geometry::Geometry)

add_test(NAME BoostTests COMMAND boost_tests)