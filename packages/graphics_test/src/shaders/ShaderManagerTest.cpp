#include <boost/test/unit_test.hpp>
#include <graphics_test/utils/TestFixture.h>
#include <graphics/Engine.h>
#include <graphics/shaders/IShader.h>
#include <graphics/shaders/VertexShader.h>
#include <graphics/shaders/ShaderManager.h>
#include <common/exceptions/IllegalArgumentException.h>

/**
 * Custom shader used for testing
 */
class TestShader : public IShader
{
public:

	/**
	 * Constructor
	 */
	TestShader() : IShader("test", "source")
	{

	}
};

BOOST_FIXTURE_TEST_SUITE(ShaderManager_suite, TestFixture);

/**
 * Tests the ability to register shaders
 */
BOOST_AUTO_TEST_CASE(ShaderManager_register)
{
	BOOST_TEST(ShaderManager::get("test") == nullptr);

	ShaderManager::registerShader(TestShader());
	BOOST_TEST(ShaderManager::get("test") != nullptr);
}

/**
 * Tests that an exception is thrown if we try to register a shader with the same name
 */
BOOST_AUTO_TEST_CASE(ShaderManager_cannotRegisterTwice)
{
	TestShader shader;
	BOOST_REQUIRE_NO_THROW(ShaderManager::registerShader(shader));
	BOOST_REQUIRE_THROW(ShaderManager::registerShader(shader), IllegalArgumentException);
}

BOOST_AUTO_TEST_SUITE_END();