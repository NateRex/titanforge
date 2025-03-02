#include <boost/test/unit_test.hpp>
#include <graphics/Engine.h>
#include <graphics/window/Window.h>
#include <graphics/Buffer.h>
#include <common/exceptions/IllegalStateException.h>

/**
 * Test the ability to obtain the current render context
 */
BOOST_AUTO_TEST_CASE(Engine_getContext)
{
	Window window = Engine::getCurrentContext();
	BOOST_TEST(window.isCurrentContext());
}

/**
 * Tests the ability to create a new window and set it as the current context
 */
BOOST_AUTO_TEST_CASE(Engine_createAndSetContext)
{
	Window window = Engine::createWindow();
	BOOST_TEST(!window.isCurrentContext());

	Engine::setContext(window);
	BOOST_TEST(window.isCurrentContext());
}

/**
 * Tests the ability to create a buffer
 */
BOOST_AUTO_TEST_CASE(Engine_createBuffer)
{
	BOOST_REQUIRE_NO_THROW(Engine::createBuffer());
}

/**
 * Tests the ability to fetch a shader by name
 */
BOOST_AUTO_TEST_CASE(Engine_getShader)
{
	// Returns nullptr on not found
	BOOST_TEST(Engine::getShader("does-not-exist") == nullptr);

	BOOST_TEST(Engine::getShader("vertex") != nullptr);
}
