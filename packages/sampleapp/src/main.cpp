#include <graphics/Engine.h>
#include <graphics/windows/WindowManager.h>
#include <graphics/windows/Window.h>
#include <graphics/windows/InputController.h>
#include <graphics/buffers/Buffer.h>
#include <graphics/buffers/BufferManager.h>
#include <graphics/textures/Texture.h>
#include <graphics/textures/TextureManager.h>
#include <graphics/shaders/Shader.h>
#include <graphics/shaders/ShaderManager.h>
#include <graphics/primitives/PPolyface.h>
#include <geometry/Vector2.h>
#include <geometry/Vector3.h>

/**
 * @return An example polyface
 */
PPolyface examplePolyface()
{
    Vector3 vertices[] = {
        Vector3(0.5f,  0.5f, 0.0f),     // top right
        Vector3(0.5f, -0.5f, 0.0f),     // bottom right
        Vector3(-0.5f, -0.5f, 0.0f),    // bottom left
        Vector3(-0.5f,  0.5f, 0.0f)     // top left 
    };
    Color colors[] = {
        Color::fromFloats(1.0f, 0.0f, 0.0f, 1.0f),
        Color::fromFloats(0.0f, 1.0f, 0.0f, 1.0f),
        Color::fromFloats(0.0f, 0.0f, 1.0f, 1.0f),
        Color::fromFloats(1.0f, 1.0f, 1.0f, 1.0f)
    };
    Vector2 texCoords[] = {
        Vector2(1, 1),
        Vector2(1, 0),
        Vector2(0, 0),
        Vector2(0, 1)
    };
    int indices[] = {
        0, 1, 3, -1,                    // first triangle
        1, 2, 3                         // second triangle
    };
    return PPolyface(vertices, 4, indices, 7, colors, texCoords);
}

/**
 * Main entrypoint for the application
 */
int main() {
    Engine::start();

    // Configure window
    Window* window = WindowManager::getCurrent();
    window->setBackgroundColor(Color::fromFloats(0.2f, 0.3f, 0.3f, 1.0f));

    // Textures
    Texture* texture = TextureManager::create("box", "assets/container.jpg");

    // Buffers
    Buffer* buffer = BufferManager::startBuffer("geometry")
        .add(examplePolyface())
        .finish();

    // Shaders
    Shader* shader = ShaderManager::get("tf_basic");
    shader->setUniform("uTexture", 0, texture);

    // Render loop
    while (window->isOpen())
    {
        Engine::startFrame();

        shader->draw(buffer);

        Engine::finishFrame();
    }

    Engine::stop();
}
