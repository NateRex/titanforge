project(GraphicsTest CXX)

# Dependencies
enable_testing()
find_package(Boost REQUIRED COMPONENTS unit_test_framework)

# Executable files
add_executable(GraphicsTest
    src/utils/TestFixture.cpp
    src/window/InputControllerTest.cpp
    src/window/WindowTest.cpp
    src/ColorTest.cpp
    src/EngineTest.cpp
    src/main.cpp
)

# Include directories
target_include_directories(
    GraphicsTest PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                    $<INSTALL_INTERFACE:include>)

# Linked libraries
target_link_libraries(GraphicsTest
    PRIVATE Boost::unit_test_framework
    PRIVATE Common::Common
    PRIVATE Graphics::Graphics)

add_test(NAME BoostTests COMMAND boost_tests)
