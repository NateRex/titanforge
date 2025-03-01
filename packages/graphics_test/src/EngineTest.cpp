#include <boost/test/unit_test.hpp>
#include <graphics_test/utils/TestFixture.h>
#include <graphics/Engine.h>
#include <graphics/window/Window.h>
#include <graphics/Buffer.h>
#include <common/exceptions/IllegalStateException.h>

/**
 * Tests the ability to start and stop the engine
 */
BOOST_AUTO_TEST_CASE(Engine_startAndStop)
{
	BOOST_REQUIRE_NO_THROW(Engine::start());
	BOOST_REQUIRE_NO_THROW(Engine::start());	// no-op

	BOOST_REQUIRE_NO_THROW(Engine::stop());
	BOOST_REQUIRE_NO_THROW(Engine::stop());		// no-op
}

/**
 * Tests that we fail to create a window if the engine is not yet initialized
 */
BOOST_AUTO_TEST_CASE(Engine_windowFailure)
{
	BOOST_REQUIRE_THROW(Engine::createWindow(), IllegalStateException);
}


BOOST_FIXTURE_TEST_SUITE(Engine_suite, TestFixture);


/**
 * Tests the ability to create a window
 */
BOOST_AUTO_TEST_CASE(Engine_createWindow)
{
	BOOST_REQUIRE_NO_THROW(Engine::createWindow());
}

/**
 * Tests the ability to create a buffer
 */
BOOST_AUTO_TEST_CASE(Engine_createBuffer)
{
	// Fails if engine has not been initialized
	BOOST_REQUIRE_THROW(Engine::createBuffer(), IllegalStateException);
	
	Engine::start();

	// Fails if no window has yet been created
	BOOST_REQUIRE_THROW(Engine::createBuffer(), IllegalStateException);

	Window window = Engine::createWindow();
	BOOST_REQUIRE_NO_THROW(Engine::createBuffer());

	Engine::stop();
}


BOOST_AUTO_TEST_SUITE_END();