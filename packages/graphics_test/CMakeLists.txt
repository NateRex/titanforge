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
    src/core/shaders/BasicShaderTest.cpp
    src/core/shaders/ShaderManagerTest.cpp
    src/core/windows/InputControllerTest.cpp
    src/core/windows/WindowTest.cpp
    src/core/BufferTest.cpp
    src/core/ColorTest.cpp
    src/core/RendererTest.cpp
    src/entities/EntityGroupTest.cpp
    src/entities/EntityTest.cpp
    src/entities/MeshTest.cpp
    src/geometry/BoxGeometryTest.cpp
    src/geometry/GeometryAttributesTest.cpp
    src/geometry/GeometryTest.cpp
    src/materials/BasicMaterialTest.cpp
    src/materials/MaterialTest.cpp
    src/scene/SceneTest.cpp
    src/textures/ImageLoaderTest.cpp
    src/textures/TextureLoaderTest.cpp
    src/textures/TextureTest.cpp
    src/GlobalTestFixture.cpp
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