#include <boost/test/unit_test.hpp>
#include <graphics_test/GlobalTestFixture.h>
#include <graphics/core/Renderer.h>
#include <graphics/core/windows/Window.h>
#include <graphics/core/windows/InputController.h>
#include <GLFW/glfw3.h>

/**
 * Tests the ability to determine whether or not a key is pressed
 */
BOOST_AUTO_TEST_CASE(InputController_isKeyPressed)
{
    WindowPtr window = GlobalTestFixture::RENDERER->getWindow();
    InputController* controller = window->getInputController();

    BOOST_TEST(!controller->isKeyPressed(GLFW_KEY_0));

    controller->setOverride(GLFW_KEY_0, GLFW_RELEASE);
    BOOST_TEST(!controller->isKeyPressed(GLFW_KEY_0));

    controller->setOverride(GLFW_KEY_0, GLFW_PRESS);
    BOOST_TEST(controller->isKeyPressed(GLFW_KEY_0));

    controller->removeOverride(GLFW_KEY_0);
    BOOST_TEST(!controller->isKeyPressed(GLFW_KEY_0));
}

/**
 * Tests the ability to add and remove listeners for specific keys
 */
BOOST_AUTO_TEST_CASE(InputController_listeners)
{
    WindowPtr window = GlobalTestFixture::RENDERER->getWindow();
    InputController* controller = window->getInputController();

    // Add listener
    int value = 0;
    controller->addListener(GLFW_KEY_A, [&value]() {
        value++;
        });

    // Key is not pressed
    controller->processInput();
    BOOST_TEST(value == 0);

    // Key is pressed
    controller->setOverride(GLFW_KEY_A, GLFW_PRESS);
    controller->processInput();
    BOOST_TEST(value == 1);

    // Remove listener
    controller->removeListeners(GLFW_KEY_A);

    // Key is pressed, but listener no longer exists
    controller->processInput();
    BOOST_TEST(value == 1);
}
