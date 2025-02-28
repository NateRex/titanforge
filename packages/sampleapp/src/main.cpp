#include <graphics/Engine.h>
#include <graphics/window/Window.h>
#include <graphics/window/InputController.h>

/**
 * Main entrypoint for the application
 */
int main() {
    Engine::start();
    Window window;

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
