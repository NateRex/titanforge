#include <boost/test/unit_test.hpp>
#include <graphics/window/InputController.h>

BOOST_FIXTURE_TEST_SUITE(InputController_suite, TestFixture);

/**
 * Tests the ability to construct an input controller and listen for user inputs
 */
BOOST_AUTO_TEST_CASE(InputController_listeners)
{
    _glfwWindow = glfwCreateWindow(800, 600, "TitanForge", NULL, NULL);
    assertNotNull(_glfwWindow, "Failed to create GLFW window", []() {
        glfwTerminate();
        });
    glfwMakeContextCurrent(_glfwWindow);

	InputController controller()
}

BOOST_AUTO_TEST_SUITE_END();