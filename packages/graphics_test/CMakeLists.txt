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
    src/cameras/CameraTest.cpp
    src/cameras/PerspectiveCameraTest.cpp
    src/core/buffers/FrameBufferTest.cpp
    src/core/buffers/GeometryBufferTest.cpp
    src/core/buffers/RenderBufferTest.cpp
    src/core/input/modifiers/InputModifiersTest.cpp
    src/core/input/modifiers/NegateModifierTest.cpp
    src/core/input/modifiers/SwizzleModifierTest.cpp
    src/core/input/DigitalInputTest.cpp
    src/core/input/InputActionTest.cpp
    src/core/input/InputContextTest.cpp
    src/core/input/InputControllerTest.cpp
    src/core/input/InputTriggerTest.cpp
    src/core/input/InputValueTest.cpp
    src/core/renderer/RendererTest.cpp
    src/core/shaders/MeshShaderTest.cpp
    src/core/shaders/ShaderManagerTest.cpp
    src/core/windows/WindowTest.cpp
    src/core/ColorTest.cpp
    src/core/EntityGroupTest.cpp
    src/core/EntityTest.cpp
    src/loaders/EntityLoaderTest.cpp
    src/loaders/ImageLoaderTest.cpp
    src/loaders/TextureLoaderTest.cpp
    src/objects/MeshTest.cpp
    src/geometry/BoxGeometryTest.cpp
    src/geometry/GeometryAttributesTest.cpp
    src/geometry/GeometryTest.cpp
    src/lights/AmbientLightTest.cpp
    src/lights/DirectionalLightTest.cpp
    src/lights/PointLightTest.cpp
    src/lights/SpotLightTest.cpp
    src/materials/MeshMaterialTest.cpp
    src/scene/SceneTest.cpp
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
add_custom_target(GraphicsTestAssets
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/assets"
        "${CMAKE_CURRENT_BINARY_DIR}/bin/assets"
    COMMENT "Copying GraphicsTest assets"
    VERBATIM
)
add_dependencies(GraphicsTest GraphicsTestAssets)

add_test(NAME BoostTests COMMAND boost_tests)
