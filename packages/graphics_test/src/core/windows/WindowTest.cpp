#include <boost/test/unit_test.hpp>
#include <graphics_test/GlobalTestFixture.h>
#include <graphics/core/Renderer.h>
#include <graphics/core/windows/Window.h>

/**
 * Tests the basic accessors of the window class
 */
BOOST_AUTO_TEST_CASE(Window_basics)
{
	WindowPtr window = GlobalTestFixture::RENDERER->getWindow();
	BOOST_TEST(window->getInputController() != nullptr);
	BOOST_TEST(window->isOpen());
}