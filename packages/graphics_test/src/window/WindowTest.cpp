#include <boost/test/unit_test.hpp>
#include <graphics/Engine.h>
#include <graphics/window/Window.h>

/**
 * Tests the ability to create a new window
 */
BOOST_AUTO_TEST_CASE(Window_basics)
{
	Window win = Engine::getCurrentWindow();
	BOOST_TEST(win.getInputController() != nullptr);
	BOOST_TEST(win.isOpen());

	win.close();
	BOOST_TEST(!win.isOpen());
}

/**
 * Tests that setting the background color results in no errors
 */
BOOST_AUTO_TEST_CASE(Window_backgroundColor)
{
	Window win = Engine::getCurrentWindow();
	BOOST_REQUIRE_NO_THROW(win.setBackgroundColor(Color::fromFloats(0.f, 0.f, 0.f, 1.f)));
}