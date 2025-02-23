#include <boost/test/unit_test.hpp>
#include <graphics_test/GraphicsTestFixture.h>
#include <graphics/Window.h>

BOOST_FIXTURE_TEST_SUITE(Window_suite, GraphicsTestFixture);

/**
 * Tests the ability to create a new window
 */
BOOST_AUTO_TEST_CASE(Window_basics)
{
	Window win;
	BOOST_TEST(win.isOpen());
	BOOST_REQUIRE_NO_THROW(win.renderFrame());

	win.close();
	BOOST_TEST(!win.isOpen());
}

BOOST_AUTO_TEST_SUITE_END();