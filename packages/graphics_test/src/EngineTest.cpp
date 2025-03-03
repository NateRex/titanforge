#include <boost/test/unit_test.hpp>
#include <graphics/Engine.h>
#include <graphics/window/Window.h>
#include <graphics/Buffer.h>
#include <common/exceptions/IllegalStateException.h>

/**
 * Test the ability to obtain the current render context
 */
BOOST_AUTO_TEST_CASE(Engine_getCurrentWindow)
{
	BOOST_TEST(Engine::getCurrentWindow() != nullptr);
}

/**
 * Tests the ability to create a new window and set it as the current context
 */
BOOST_AUTO_TEST_CASE(Engine_createAndSetContext)
{
	Window window = Engine::createWindow();
	BOOST_REQUIRE_NO_THROW(Engine::setCurrentWindow(window));
}

/**
 * Tests the ability to create a buffer
 */
BOOST_AUTO_TEST_CASE(Engine_createBuffer)
{
	BOOST_REQUIRE_NO_THROW(Engine::createBuffer());
}

/**
 * Tests that no errors occur rendering a frame for the current window context
 */
BOOST_AUTO_TEST_CASE(Engine_render)
{
	BOOST_REQUIRE_NO_THROW(Engine::renderFrame());
}
