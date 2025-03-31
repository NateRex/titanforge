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
    src/entities/EntityTest.cpp
    src/geometry/BoxGeometryTest.cpp
    src/geometry/GeometryTest.cpp
    src/shaders/ShaderManagerTest.cpp
    src/shaders/ShaderTest.cpp
    src/testutils/GlobalTestFixture.cpp
    src/testutils/ShaderTestFixture.cpp
    src/textures/ImageLoaderTest.cpp
    src/textures/TextureManagerTest.cpp
    src/textures/TextureTest.cpp
    src/windows/InputControllerTest.cpp
    src/windows/WindowManagerTest.cpp
    src/windows/WindowTest.cpp
    src/ColorTest.cpp
    src/EngineTest.cpp
    src/main.cpp
)

# Include directories
target_include_directories(GraphicsTest
    PRIVATE ${Stb_INCLUDE_DIR}
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PUBLIC $<INSTALL_INTERFACE:include>)

# Linked libraries
target_link_libraries(GraphicsTest
    PUBLIC Boost::unit_test_framework
    PUBLIC Common::Common
    PUBLIC Math::Math
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