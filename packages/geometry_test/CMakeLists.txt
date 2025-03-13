project(GeometryTest CXX)

set(CMAKE_BUILD_TYPE Debug)
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
endif()

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
    PUBLIC Boost::unit_test_framework
    PUBLIC Common::Common
    PUBLIC Geometry::Geometry)

# Outputs
set_target_properties(GeometryTest PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${CMAKE_CURRENT_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_CURRENT_BINARY_DIR}/bin"
)

add_test(NAME BoostTests COMMAND boost_tests)