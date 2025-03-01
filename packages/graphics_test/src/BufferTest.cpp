#include <boost/test/unit_test.hpp>
#include <graphics_test/utils/TestFixture.h>
#include <graphics/Engine.h>
#include <graphics/Buffer.h>

BOOST_FIXTURE_TEST_SUITE(InputController_suite, TestFixture);

/**
 * Tests the ability to add vertex data to a buffer
 */
BOOST_AUTO_TEST_CASE(Buffer_addVertices)
{
	Buffer buffer = Engine::createBuffer();

	float vertices[] = {
		0.1, 0.2, 0.3,
		0.4, 0.5, 0.6
	};

	BOOST_REQUIRE_NO_THROW(buffer.addVertices(vertices, 6));
}

BOOST_AUTO_TEST_SUITE_END();