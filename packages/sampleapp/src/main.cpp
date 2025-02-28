#include <graphics/Engine.h>
#include <graphics/window/Window.h>
#include <graphics/window/InputController.h>
#include <graphics/primitives/PPolyface.h>

/**
 * Create an example polyface
 */
PPolyface examplePolyface()
{
    return PolyfacePrimitive()
}

/**
 * Main entrypoint for the application
 */
int main() {
    Engine::start();
    Window window;
    window.setBackgroundColor(Color::fromFloats(0.2f, 0.3f, 0.3f, 1.0f));

    // Create input listeners
    window.getInputController()->addListener(256, [&window]() {
        window.close();
    });

    while (window.isOpen())
    {

        window.renderFrame();
    }

    Engine::stop();
}
