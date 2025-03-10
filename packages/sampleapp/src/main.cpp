#include <graphics/Engine.h>
#include <graphics/window/Window.h>
#include <graphics/window/InputController.h>
#include <graphics/buffers/BufferManager.h>
#include <graphics/shaders/ShaderManager.h>
#include <graphics/primitives/PPolyface.h>
#include <geometry/Vector3.h>
#include <glad/glad.h>

/**
 * @return An example polyface
 */
PPolyface examplePolyface()
{
    Vector3 vertices[] = {
        Vector3(0.6f,  0.5f, 0.0f),     // top right
        Vector3(0.5f, -0.5f, 0.0f),     // bottom right
        Vector3(-0.6f, -0.5f, 0.0f),    // bottom left
        Vector3(-0.5f,  0.5f, 0.0f)     // top left 
    };
    Color colors[] = {
        Color::fromFloats(1.0f, 0.0f, 0.0f, 1.0f),
        Color::fromFloats(0.0f, 1.0f, 0.0f, 1.0f),
        Color::fromFloats(0.0f, 0.0f, 1.0f, 1.0f),
        Color::fromFloats(1.0f, 1.0f, 1.0f, 1.0f)
    };
    int indices[] = {
        0, 1, 3, -1,                    // first triangle
        1, 2, 3                         // second triangle
    };
    return PPolyface(vertices, 4, indices, 7, colors);
}

/**
 * Main entrypoint for the application
 */
int main() {
    Engine::start();

    // Configure window
    Window* window = Engine::getCurrentWindow();
    window->setBackgroundColor(Color::fromFloats(0.2f, 0.3f, 0.3f, 1.0f));


    // Create input listeners
    InputController* inputController = window->getInputController();
    inputController->addListener(256, [&window]() {
        window->close();
    });


    // Create buffer
    PPolyface geometry = examplePolyface();
    BufferManager::startBuffer("geometry")
        .add(geometry)
        .finish();


    // Render loop
    while (window->isOpen())
    {
        Engine::startFrame();

        // Draw geometry
        BufferManager::draw("geometry");

        Engine::finishFrame();
    }

    Engine::stop();
}
