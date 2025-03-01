#include <boost/test/unit_test.hpp>
#include <graphics_test/utils/TestFixture.h>
#include <graphics/Engine.h>
#include <graphics/shaders/IShader.h>


BOOST_FIXTURE_TEST_SUITE(VertexShader_suite, TestFixture);

/**
 * Ensure the ability to construct and fetch a VertexShader instance via the graphics engine
 */
BOOST_AUTO_TEST_CASE(VertexShader_basics)
{
	const std::string name("vertex");
	const IShader* shader = Engine::getShader(name);

	BOOST_TEST(shader != nullptr);
	BOOST_TEST(shader->getName() == name);
	BOOST_TEST(shader->getSrc().length() > 0);
}

BOOST_AUTO_TEST_SUITE_END();