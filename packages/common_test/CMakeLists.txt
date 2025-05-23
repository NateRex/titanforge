project(CommonTest CXX)

set(CMAKE_BUILD_TYPE Debug)
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
endif()

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
    src/exceptions/UnsupportedOperationExceptionTest.cpp
    src/AssertionsTest.cpp
    src/main.cpp
    src/UtilsTest.cpp)

# Linked libraries
target_link_libraries(CommonTest
    PUBLIC Boost::unit_test_framework
    PUBLIC Common::Common)

# Outputs
set_target_properties(CommonTest PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${CMAKE_CURRENT_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_CURRENT_BINARY_DIR}/bin"
)

add_test(NAME BoostTests COMMAND boost_tests)