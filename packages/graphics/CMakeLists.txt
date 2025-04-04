project(Graphics CXX)

# Dependencies
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(Stb REQUIRED)

# Library files
add_library(Graphics
    src/core/shaders/Shader.cpp
    src/core/shaders/ShaderManager.cpp
    src/core/windows/InputController.cpp
    src/core/windows/Window.cpp
    src/core/windows/WindowManager.cpp
    src/core/Buffer.cpp
    src/core/ImageLoader.cpp
    src/entities/Entity.cpp
    src/entities/Mesh.cpp
    src/geometry/BoxGeometry.cpp
    src/geometry/Geometry.cpp
    src/geometry/GeometryAttributes.cpp
    src/materials/BasicMaterial.cpp
    src/materials/Material.cpp
    src/textures/Texture.cpp
    src/textures/TextureManager.cpp
    src/Color.cpp
    src/Engine.cpp
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