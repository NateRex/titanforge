project(Graphics CXX)

# Dependencies
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)

# Library files
add_library(Graphics
    src/buffers/Buffer.cpp
    src/buffers/BufferManager.cpp
    src/primitives/PPolyface.cpp
    src/primitives/PrimitiveAttributes.cpp
    src/shaders/Shader.cpp
    src/shaders/ShaderManager.cpp
    src/shaders/ShaderProgram.cpp
    src/window/InputController.cpp
    src/window/Window.cpp
    src/Color.cpp
    src/Engine.cpp
)
add_library(Graphics::Graphics ALIAS Graphics)

# Include directories
target_include_directories(
    Graphics PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                    $<INSTALL_INTERFACE:include>)

# Linked libraries
target_link_libraries(Graphics
    PRIVATE glfw
    PRIVATE glad::glad
    PRIVATE Common::Common
    PRIVATE Geometry::Geometry)

# Library installation
install(
    TARGETS Graphics
    EXPORT GraphicsConfig
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION lib
    INCLUDES DESTINATION include)