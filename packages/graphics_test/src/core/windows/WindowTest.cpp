#include <boost/test/unit_test.hpp>
#include <graphics_test/GlobalTestFixture.h>
#include <graphics/core/windows/Window.h>

/**
 * Tests the ability to fetch the input controller
 */
BOOST_AUTO_TEST_CASE(Window_inputController)
{
	Renderer* renderer = GlobalTestFixture::RENDERER;
	WindowPtr window = renderer->getWindow();
	BOOST_TEST(window->getInputController() != nullptr);
}

/**
 * Tests the ability to check whether a window is still open
 */
BOOST_AUTO_TEST_CASE(Window_isOpen)
{
	Renderer* renderer = GlobalTestFixture::RENDERER;
	WindowPtr window = renderer->getWindow();
	BOOST_TEST(window->isOpen());
}