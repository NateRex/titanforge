#include <boost/test/unit_test.hpp>
#include <graphics_test/TestUtils.h>
#include <graphics/shaders/Shader.h>
#include <graphics/shaders/ShaderProgram.h>
#include <graphics/shaders/ShaderManager.h>
#include <common/exceptions/IllegalArgumentException.h>

/**
 * Tests the ability to mount and unmount shaders
 */
BOOST_AUTO_TEST_CASE(ShaderManager_mount)
{
	Shader shader = createExampleShader("ShaderManagerTest_Shader1");
	BOOST_REQUIRE_NO_THROW(ShaderManager::mountShader(shader));
	BOOST_REQUIRE_NO_THROW(ShaderManager::unmountShaders());
}

/**
 * Tests that an exception is thrown if we try to mount a shader with the same name twice
 */
BOOST_AUTO_TEST_CASE(ShaderManager_cannotMountTwice)
{
	Shader shader = createExampleShader("ShaderManagerTest_Shader2");
	BOOST_REQUIRE_NO_THROW(ShaderManager::mountShader(shader));
	BOOST_REQUIRE_THROW(ShaderManager::mountShader(shader), IllegalArgumentException);
}

/**
 * Tests the ability to link and use a program
 */
BOOST_AUTO_TEST_CASE(ShaderManager_linkAndUse)
{
	Shader shader = createExampleShader("ShaderManagerTest_Shader3");
	ShaderProgram prgm("ShaderManagerTest_Prgm1", { "ShaderManagerTest_Shader3" });

	ShaderManager::mountShader(shader);
	BOOST_REQUIRE_NO_THROW(ShaderManager::linkProgram(prgm));
	ShaderManager::unmountShaders();

	BOOST_REQUIRE_NO_THROW(ShaderManager::useProgram(prgm.getName()));

	// calling use a second time results in no-op
	BOOST_REQUIRE_NO_THROW(ShaderManager::useProgram(prgm.getName()));
}

/**
 * Tests that an exception is thrown if we try to link a program with the same name twice
 */
BOOST_AUTO_TEST_CASE(ShaderManager_cannotLinkTwice)
{
	Shader shader = createExampleShader("ShaderManagerTest_Shader4");
	ShaderProgram prgm("ShaderManagerTest_Prgm2", { "ShaderManagerTest_Shader4" });

	ShaderManager::mountShader(shader);
	BOOST_REQUIRE_NO_THROW(ShaderManager::linkProgram(prgm));
	BOOST_REQUIRE_THROW(ShaderManager::linkProgram(prgm), IllegalArgumentException);
	ShaderManager::unmountShaders();
}

/**
 * Tests that an exception is thrown if we try to link a program reference shaders that have not
 * been mounted
 */
BOOST_AUTO_TEST_CASE(ShaderManager_mustMountShadersToLink)
{
	ShaderProgram prgm("ShaderManagerTest_Prgm3", { "does-not-exist" });
	BOOST_REQUIRE_THROW(ShaderManager::linkProgram(prgm), IllegalArgumentException);
}