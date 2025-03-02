#include <boost/test/unit_test.hpp>
#include <graphics/Engine.h>
#include <graphics/window/Window.h>
#include <graphics/window/InputController.h>
#include <GLFW/glfw3.h>

/**
 * Tests the ability to determine whether or not a key is pressed
 */
BOOST_AUTO_TEST_CASE(InputController_isKeyPressed)
{
    Window window = Engine::getCurrentContext();
    InputController* controller = window.getInputController();
    
    BOOST_TEST(!controller->isKeyPressed(GLFW_KEY_0));

    controller->setOverride(GLFW_KEY_0, GLFW_RELEASE);
    BOOST_TEST(!controller->isKeyPressed(GLFW_KEY_0));

    controller->setOverride(GLFW_KEY_0, GLFW_PRESS);
    BOOST_TEST(controller->isKeyPressed(GLFW_KEY_0));

    controller->removeOverride(GLFW_KEY_0);
    BOOST_TEST(!controller->isKeyPressed(GLFW_KEY_0));
}

/**
 * Tests the ability to construct an input controller and listen for user inputs
 */
BOOST_AUTO_TEST_CASE(InputController_listeners)
{
    Window window = Engine::getCurrentContext();
    InputController* controller = window.getInputController();
    
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
}
