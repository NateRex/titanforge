#include <boost/test/unit_test.hpp>
#include <graphics/shaders/Shader.h>
#include <graphics/shaders/ShaderManager.h>
#include <common/exceptions/InstantiationException.h>
#include <glad/glad.h>

/**
 * Shader with invalid source code, used for testing
 */
class ShaderTest_Shader : public Shader
{
public:

	/**
	 * Constructor
	 * @param name Unique name to give to this shader
	 */
	ShaderTest_Shader(const std::string& name) : Shader(GL_VERTEX_SHADER, name, R"(
		#version 330 core

		layout (location = 0) in vec3 aPos;

		void main()
		{
			gl_Position = vec1000(aPos.x, aPos.y, aPos.z, 1.0);
		}
		)")
	{

	}
};

/**
 * Test the basic accessors of the class
 */
BOOST_AUTO_TEST_CASE(Shader_basics)
{
	ShaderTest_Shader shader("ShaderTest_Shader1");
	BOOST_TEST(shader.getName() == "ShaderTest_Shader1");
	BOOST_TEST(shader.getSrc().length() > 0);
}

/**
 * Tests that an InstantiationException is thrown on failure to create/compile a shader
 */
BOOST_AUTO_TEST_CASE(Shader_compilationFailure)
{
	ShaderTest_Shader shader("ShaderTest_Shader2");
    BOOST_REQUIRE_THROW(ShaderManager::loadShader(shader), InstantiationException);
}