#include <boost/test/unit_test.hpp>
#include <graphics/shaders/Shader.h>
#include <graphics/shaders/ShaderManager.h>
#include <common/exceptions/IllegalArgumentException.h>
#include <glad/glad.h>

/**
 * Custom shader used for testing
 */
class ShaderManagerTest_Shader : public Shader
{
public:

	/**
	 * Constructor
	 * @param name Unique name to give to this shader
	 */
	ShaderManagerTest_Shader(const std::string& name) : Shader(GL_VERTEX_SHADER, name, R"(
		#version 330 core

		layout (location = 0) in vec3 aPos;

		void main()
		{
			gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
		}
		)")
	{

	}
};

/**
 * Tests the ability to register shaders
 */
BOOST_AUTO_TEST_CASE(ShaderManager_register)
{
	BOOST_TEST(ShaderManager::get("ShaderManagerTest_Shader1") == nullptr);

	ShaderManager::loadShader(ShaderManagerTest_Shader("ShaderManagerTest_Shader1"));
	BOOST_TEST(ShaderManager::get("ShaderManagerTest_Shader1") != nullptr);
}

/**
 * Tests that an exception is thrown if we try to register a shader with the same name
 */
BOOST_AUTO_TEST_CASE(ShaderManager_cannotRegisterTwice)
{
	ShaderManagerTest_Shader shader("ShaderManagerTest_Shader2");
	BOOST_REQUIRE_NO_THROW(ShaderManager::loadShader(shader));
	BOOST_REQUIRE_THROW(ShaderManager::loadShader(shader), IllegalArgumentException);
}