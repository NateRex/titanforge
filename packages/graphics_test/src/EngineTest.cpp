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

	BOOST_REQUIRE_NO_THROW(Engine::stop());
}

/**
 * Tests that we fail to create a window if the engine is not yet initialized
 */
BOOST_AUTO_TEST_CASE(Engine_windowFailure)
{
	BOOST_REQUIRE_THROW(Engine::createWindow(), IllegalStateException);
}

/**
 * Tests that we fail to create a buffer if the engine is not yet initialized
 */
BOOST_AUTO_TEST_CASE(Engine_bufferFailure)
{
	BOOST_REQUIRE_THROW(Engine::createBuffer(), IllegalStateException);
}

/**
 * Tests that we fail to obtain a reference to a shader if the engine is not yet initialized
 */
BOOST_AUTO_TEST_CASE(Engine_shaderFailure)
{
	BOOST_REQUIRE_THROW(Engine::getShader("vertex"), IllegalStateException);
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
	// Fails if no window has yet been created
	BOOST_REQUIRE_THROW(Engine::createBuffer(), IllegalStateException);

	Window window = Engine::createWindow();

	BOOST_REQUIRE_NO_THROW(Engine::createBuffer());
}

/**
 * Tests the ability to fetch a shader by name
 */
BOOST_AUTO_TEST_CASE(Engine_getShader)
{
	// Fails if no window has yet been created
	BOOST_REQUIRE_THROW(Engine::getShader("vertex"), IllegalStateException);

	Window window = Engine::createWindow();

	// Returns nullptr on not found
	BOOST_TEST(Engine::getShader("does-not-exist") == nullptr);

	BOOST_TEST(Engine::getShader("vertex") != nullptr);
}


BOOST_AUTO_TEST_SUITE_END();