project(Graphics CXX)

# Dependencies
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(Stb REQUIRED)
find_package(assimp CONFIG REQUIRED)

# Library files
add_library(Graphics
    src/cameras/Camera.cpp
    src/cameras/PerspectiveCamera.cpp
    src/core/buffers/FrameBuffer.cpp
    src/core/buffers/GeometryBuffer.cpp
    src/core/buffers/RenderBuffer.cpp
    src/core/input/modifiers/InputModifiers.cpp
    src/core/input/modifiers/NegateModifier.cpp
    src/core/input/modifiers/SwizzleModifier.cpp
    src/core/input/DigitalInput.cpp
    src/core/input/InputAction.cpp
    src/core/input/InputContext.cpp
    src/core/input/InputController.cpp
    src/core/input/InputTrigger.cpp
    src/core/input/InputValue.cpp
    src/core/renderer/Renderer.cpp
    src/core/renderer/RenderTarget.cpp
    src/core/shaders/MeshShader.cpp
	src/core/shaders/PostProcessShader.cpp
    src/core/shaders/Shader.cpp
    src/core/shaders/ShaderManager.cpp
    src/core/windows/Window.cpp
    src/core/Color.cpp
    src/core/Entity.cpp
    src/core/EntityGroup.cpp
    src/core/PixelFormats.cpp
    src/loaders/ImageLoader.cpp
    src/loaders/ModelLoader.cpp
    src/loaders/TextureLoader.cpp
    src/objects/Mesh.cpp
    src/geometry/BoxGeometry.cpp
    src/geometry/Geometry.cpp
    src/geometry/GeometryAttributes.cpp
    src/lights/AmbientLight.cpp
    src/lights/DirectionalLight.cpp
    src/lights/Light.cpp
    src/lights/PointLight.cpp
    src/lights/SpotLight.cpp
    src/materials/Material.cpp
    src/materials/MeshMaterial.cpp
	src/materials/PostProcessMaterial.cpp
    src/materials/SkyboxMaterial.cpp
    src/scene/Scene.cpp
    src/textures/Texture.cpp
    src/textures/Texture2D.cpp
    src/textures/TextureCube.cpp
    src/textures/TextureFormats.cpp
)
add_library(Graphics::Graphics ALIAS Graphics)

# Include directories
target_include_directories(Graphics
    PRIVATE ${Stb_INCLUDE_DIR}
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PUBLIC $<INSTALL_INTERFACE:include>)

# Linked libraries
target_link_libraries(Graphics
    PRIVATE glfw
    PRIVATE glad::glad
    PRIVATE assimp::assimp
    PRIVATE Common::Common
    PRIVATE Math::Math)

# Outputs
set_target_properties(Graphics PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin"
    ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_BINARY_DIR}/bin"
    ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_BINARY_DIR}/bin"
    ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL "${CMAKE_CURRENT_BINARY_DIR}/bin"
    ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_CURRENT_BINARY_DIR}/bin"
)
