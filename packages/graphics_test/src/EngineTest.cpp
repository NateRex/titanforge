#include <boost/test/unit_test.hpp>
#include <graphics/Engine.h>
#include <graphics/windows/Window.h>
#include <common/exceptions/IllegalStateException.h>

/**
 * Test the ability to obtain the time since the engine has been initialized
 */
BOOST_AUTO_TEST_CASE(Engine_getTime)
{
	BOOST_TEST(Engine::getTime() > 0.);
}

/**
 * Tests that no errors occur rendering a frame for the current window context
 */
BOOST_AUTO_TEST_CASE(Engine_render)
{
	BOOST_REQUIRE_NO_THROW(Engine::startFrame());
	BOOST_REQUIRE_NO_THROW(Engine::finishFrame());
}
