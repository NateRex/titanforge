project(GraphicsTest CXX)

set(CMAKE_BUILD_TYPE Debug)
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
endif()

# Dependencies
enable_testing()
find_package(Boost REQUIRED COMPONENTS unit_test_framework)

# Executable files
add_executable(GraphicsTest
    src/buffers/BufferManagerTest.cpp
    src/buffers/BufferTest.cpp
    src/primitives/PPolyfaceTest.cpp
    src/primitives/PrimitiveAttributesTest.cpp
    src/shaders/ShaderManagerTest.cpp
    src/shaders/ShaderTest.cpp
    src/textures/ImageLoaderTest.cpp
    src/textures/TextureManagerTest.cpp
    src/textures/TextureTest.cpp
    src/window/InputControllerTest.cpp
    src/window/WindowTest.cpp
    src/ColorTest.cpp
    src/EngineTest.cpp
    src/main.cpp
)

# Linked libraries
target_link_libraries(GraphicsTest
    PUBLIC Boost::unit_test_framework
    PUBLIC Common::Common
    PUBLIC Geometry::Geometry
    PUBLIC Graphics::Graphics)

# Outputs
set_target_properties(GraphicsTest PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${CMAKE_CURRENT_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_CURRENT_BINARY_DIR}/bin"
)
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/assets" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/bin")

add_test(NAME BoostTests COMMAND boost_tests)