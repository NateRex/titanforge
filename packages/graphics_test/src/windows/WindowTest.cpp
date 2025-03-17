#include <boost/test/unit_test.hpp>
#include <graphics/windows/WindowManager.h>
#include <graphics/windows/Window.h>

/**
 * Tests construction and destruction of a window using the window manager. Also tests the basic accessors
 * of the class.
 */
BOOST_AUTO_TEST_CASE(Window_basics)
{
	Window* win = WindowManager::create("window");
	BOOST_TEST(win->name == "window");
	BOOST_TEST(win->isOpen());
	BOOST_TEST(win->getInputController() != nullptr);
	BOOST_REQUIRE_NO_THROW(WindowManager::destroy("window"));
}

/**
 * Tests that setting the background color results in no errors
 */
BOOST_AUTO_TEST_CASE(Window_backgroundColor)
{
	Window* win = WindowManager::getCurrent();
	BOOST_REQUIRE_NO_THROW(win->setBackgroundColor(Color::fromFloats(0.f, 0.f, 0.f, 1.f)));
}