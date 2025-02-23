#include <graphics/Engine.h>
#include <graphics/Window.h>

/**
 * Main entrypoint for the application
 */
int main() {
    Engine::start();

    Window window;
    while (!window.closed())
    {
        window.renderFrame();
    }

    Engine::stop();
}
