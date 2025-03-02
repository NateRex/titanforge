project(GraphicsTest CXX)

# Dependencies
enable_testing()
find_package(Boost REQUIRED COMPONENTS unit_test_framework)

# Executable files
add_executable(GraphicsTest
    src/primitives/PPolyfaceTest.cpp
    src/shaders/IShaderTest.cpp
    src/shaders/ShaderManagerTest.cpp
    src/shaders/VertexShaderTest.cpp
    src/window/InputControllerTest.cpp
    src/window/WindowTest.cpp
    src/BufferTest.cpp
    src/ColorTest.cpp
    src/EngineTest.cpp
    src/main.cpp
)

# Linked libraries
target_link_libraries(GraphicsTest
    PRIVATE Boost::unit_test_framework
    PRIVATE Common::Common
    PRIVATE Geometry::Geometry
    PRIVATE Graphics::Graphics)

add_test(NAME BoostTests COMMAND boost_tests)