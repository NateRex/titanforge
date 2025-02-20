project(CommonTest CXX)

enable_testing()
find_package(Boost REQUIRED COMPONENTS unit_test_framework)

add_executable(CommonTest
    src/exceptions/IllegalArgumentExceptionTest.cpp
    src/exceptions/OutOfBoundsExceptionTest.cpp
    src/main.cpp
    src/UtilsTest.cpp)

target_link_libraries(CommonTest
    PRIVATE Boost::unit_test_framework
    PRIVATE Common::Common)

add_test(NAME BoostTests COMMAND boost_tests)