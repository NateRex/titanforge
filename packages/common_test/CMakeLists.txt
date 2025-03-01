project(CommonTest CXX)

# Dependencies
enable_testing()
find_package(Boost REQUIRED COMPONENTS unit_test_framework)

# Executable files
add_executable(CommonTest
    src/exceptions/IllegalArgumentExceptionTest.cpp
    src/exceptions/IllegalStateExceptionTest.cpp
    src/exceptions/InstantiationExceptionTest.cpp
    src/exceptions/NullPointerExceptionTest.cpp
    src/exceptions/OutOfBoundsExceptionTest.cpp
    src/AssertionsTest.cpp
    src/main.cpp
    src/UtilsTest.cpp)

# Linked libraries
target_link_libraries(CommonTest
    PRIVATE Boost::unit_test_framework
    PRIVATE Common::Common)

add_test(NAME BoostTests COMMAND boost_tests)