#include <boost/test/unit_test.hpp>
#include <graphics/Engine.h>

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