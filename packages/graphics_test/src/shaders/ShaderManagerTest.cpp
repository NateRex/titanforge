#include <boost/test/unit_test.hpp>
#include <graphics/shaders/Shader.h>
#include <graphics/shaders/ShaderManager.h>
#include <graphics/shaders/glsl/Basic.h>
#include <common/exceptions/IllegalArgumentException.h>
#include <glad/glad.h>

/**
 * Tests the ability to create and fetch a shader
 */
BOOST_AUTO_TEST_CASE(ShaderManager_createAndFetch)
{
	ShaderManager::create("shader", Shaders::BASIC_VERTEX, Shaders::BASIC_FRAGMENT);
	BOOST_TEST(ShaderManager::get("shader") != nullptr);
	BOOST_REQUIRE_NO_THROW(ShaderManager::destroy("shader"));
}

/**
 * Tests that an exception is thrown if we try to create a shader that already exists
 */
BOOST_AUTO_TEST_CASE(ShaderManager_cannotCreateDuplicates)
{
	ShaderManager::create("shader", Shaders::BASIC_VERTEX, Shaders::BASIC_FRAGMENT);
	BOOST_REQUIRE_THROW(ShaderManager::create("shader", Shaders::BASIC_VERTEX, Shaders::BASIC_FRAGMENT),
			IllegalArgumentException);

	ShaderManager::destroy("shader");
}

/**
 * Tests that an exception is thrown if we try to fetch a shader that does not exist
 */
BOOST_AUTO_TEST_CASE(ShaderManager_cannotUseMissing)
{
	BOOST_REQUIRE_THROW(ShaderManager::get("does-not-exist"), IllegalArgumentException);
}

/**
 * Tests that an exception is thrown if we try to destroy a shader that does not exist
 */
BOOST_AUTO_TEST_CASE(ShaderManager_cannotDestroyMissing)
{
	BOOST_REQUIRE_THROW(ShaderManager::destroy("does-not-exist"), IllegalArgumentException);

	ShaderManager::create("shader", Shaders::BASIC_VERTEX, Shaders::BASIC_FRAGMENT);
	BOOST_REQUIRE_NO_THROW(ShaderManager::destroy("shader"));
	BOOST_REQUIRE_THROW(ShaderManager::destroy("shader"), IllegalArgumentException);
}
