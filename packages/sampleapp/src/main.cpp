#include <graphics/Engine.h>
#include <graphics/window/Window.h>

/**
 * Main entrypoint for the application
 */
int main() {
    Engine::start();

    Window window;
    while (window.isOpen())
    {
        window.renderFrame();
    }

    Engine::stop();
}
