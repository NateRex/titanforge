#include <boost/test/unit_test.hpp>
#include <graphics_test/TestUtils.h>
#include <graphics/shaders/Shader.h>
#include <graphics/shaders/ShaderProgram.h>
#include <graphics/shaders/ShaderManager.h>
#include <common/exceptions/IllegalArgumentException.h>
#include <glad/glad.h>

/**
 * Tests the ability to mount and unmount shaders
 */
BOOST_AUTO_TEST_CASE(ShaderManager_mount)
{
	Shader shader = createExampleVertexShader("ShaderManagerTest_Shader1");
	BOOST_REQUIRE_NO_THROW(ShaderManager::mountShader(shader));
	BOOST_REQUIRE_NO_THROW(ShaderManager::unmountShaders());
}

/**
 * Tests that an exception is thrown if we try to mount a shader with the same name twice
 */
BOOST_AUTO_TEST_CASE(ShaderManager_cannotMountTwice)
{
	Shader shader = createExampleVertexShader("ShaderManagerTest_Shader2");
	BOOST_REQUIRE_NO_THROW(ShaderManager::mountShader(shader));
	BOOST_REQUIRE_THROW(ShaderManager::mountShader(shader), IllegalArgumentException);
}

/**
 * Tests the ability to link and use a program
 */
BOOST_AUTO_TEST_CASE(ShaderManager_linkAndUse)
{
	Shader shader = createExampleVertexShader("ShaderManagerTest_Shader3");
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
	Shader shader = createExampleVertexShader("ShaderManagerTest_Shader4");
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

/**
 * Tests the ability to set a uniform value
 */
BOOST_AUTO_TEST_CASE(ShaderManager_setUniform)
{
	Shader shader1 = createExampleVertexShader("ShaderManagerTest_Shader5");
	Shader shader2 = createExampleFragmentShader("ShaderManagerTest_Shader6");
	ShaderProgram prgm("ShaderManagerTest_Prgm4", { "ShaderManagerTest_Shader5", "ShaderManagerTest_Shader6"});

	ShaderManager::mountShader(shader1);
	ShaderManager::mountShader(shader2);
	ShaderManager::linkProgram(prgm);
	ShaderManager::unmountShaders();

	BOOST_REQUIRE_NO_THROW(ShaderManager::setUniform("ShaderManagerTest_Prgm4", "color",
		glUniform4f, 0.1f, 0.2f, 0.3f, 0.4f));
}

/**
 * Tests that we cannot set the uniform value of a shader program that does not exist
 */
BOOST_AUTO_TEST_CASE(ShaderManager_setUniformMissingProgram)
{
	BOOST_REQUIRE_THROW(ShaderManager::setUniform("does-not-exist", "uniformVariable",
		glUniform4f, 0.1f, 0.2f, 0.3f, 0.4f), IllegalArgumentException);
}

/**
 * Tests that we cannot set the value of a uniform variable that does not exist inside of a shader program
 */
BOOST_AUTO_TEST_CASE(ShaderManager_setUniformMissingVariable)
{
	Shader shader = createExampleVertexShader("ShaderManagerTest_Shader7");
	ShaderProgram prgm("ShaderManagerTest_Prgm5", { "ShaderManagerTest_Shader7" });

	ShaderManager::mountShader(shader);
	ShaderManager::linkProgram(prgm);
	ShaderManager::unmountShaders();

	BOOST_REQUIRE_THROW(ShaderManager::setUniform("ShaderManagerTest_Prgm5", "does-not-exist",
		glUniform4f, 0.1f, 0.2f, 0.3f, 0.4f), IllegalArgumentException);
}