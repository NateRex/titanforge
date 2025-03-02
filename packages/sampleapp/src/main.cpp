#include <graphics/Engine.h>
#include <graphics/window/Window.h>
#include <graphics/window/InputController.h>
#include <graphics/primitives/PPolyface.h>
#include <geometry/Vector3.h>

/**
 * Create an example polyface
 */
PPolyface examplePolyface()
{
    Vector3 pts[] = {
        Vector3(-0.5, -0.5, 0.),
        Vector3(0.5, -0.5, 0.),
        Vector3(0., 0.5, 0.)
    };
    int verts[] = {
        0, 1, 2
    };
    return PPolyface(pts, 3, verts, 3);
}

/**
 * Main entrypoint for the application
 */
int main() {
    Engine::start();
    Window window = Engine::getCurrentWindow();
    window.setBackgroundColor(Color::fromFloats(0.2f, 0.3f, 0.3f, 1.0f));

    // Create input listeners
    window.getInputController()->addListener(256, [&window]() {
        window.close();
    });

    // Get example geometry
    PPolyface geometry = examplePolyface();

    // Render loop
    while (window.isOpen())
    {
        window.renderFrame();
    }

    Engine::stop();
}
