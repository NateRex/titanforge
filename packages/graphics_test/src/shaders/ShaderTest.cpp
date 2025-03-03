#include <boost/test/unit_test.hpp>
#include <graphics_test/TestUtils.h>
#include <graphics/shaders/Shader.h>
#include <graphics/shaders/ShaderManager.h>
#include <common/exceptions/InstantiationException.h>
#include <glad/glad.h>

/**
 * Test the basic accessors of the class
 */
BOOST_AUTO_TEST_CASE(Shader_basics)
{
	Shader shader = createExampleShader("ShaderTest_Shader1");
	BOOST_TEST(shader.getName() == "ShaderTest_Shader1");
	BOOST_TEST(shader.getSrc().length() > 0);
}

/**
 * Tests that an InstantiationException is thrown on failure to compile a shader
 */
BOOST_AUTO_TEST_CASE(Shader_compilationFailure)
{
	Shader invalidShader(GL_VERTEX_SHADER, "ShaderTest_Shader2", R"(
		#version 330 core

		layout (location = 0) in vec3 aPos;

		void main()
		{
			gl_Position = vec1000(aPos.x, aPos.y, aPos.z, 1.0);
		}
		)");
    BOOST_REQUIRE_THROW(ShaderManager::mountShader(invalidShader), InstantiationException);
}