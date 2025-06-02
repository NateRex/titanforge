project(Graphics CXX)

# Dependencies
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(Stb REQUIRED)

# Library files
add_library(Graphics
    src/cameras/Camera.cpp
    src/cameras/PerspectiveCamera.cpp
    src/core/input/InputAction.cpp
    src/core/input/InputContext.cpp
    src/core/input/InputController.cpp
    src/core/input/InputKey.cpp
    src/core/input/InputTrigger.cpp
    src/core/input/InputValue.cpp
    src/core/shaders/BasicShader.cpp
    src/core/shaders/Shader.cpp
    src/core/shaders/ShaderManager.cpp
    src/core/windows/Window.cpp
    src/core/Buffer.cpp
    src/core/Color.cpp
    src/core/Renderer.cpp
    src/entities/Entity.cpp
    src/entities/EntityGroup.cpp
    src/entities/Mesh.cpp
    src/geometry/BoxGeometry.cpp
    src/geometry/Geometry.cpp
    src/geometry/GeometryAttributes.cpp
    src/materials/BasicMaterial.cpp
    src/materials/Material.cpp
    src/scene/Scene.cpp
    src/textures/ImageLoader.cpp
    src/textures/Texture.cpp
    src/textures/TextureLoader.cpp
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