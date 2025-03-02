#include <boost/test/unit_test.hpp>
#include <graphics/shaders/IShader.h>
#include <graphics/shaders/VertexShader.h>
#include <graphics/shaders/ShaderManager.h>
#include <common/exceptions/InstantiationException.h>
#include <glad/glad.h>

/**
 * Shader with invalid source code, used for testing
 */
class InvalidShader : public IShader
{
public:

	/**
	 * Constructor
	 * @param name Unique name to give to this shader
	 */
	InvalidShader(const std::string& name) : IShader(GL_VERTEX_SHADER, name, R"(
		#version 330 core

		layout (location = 0) in vec3 aPos;

		void main()
		{
			gl_Position = vec5(aPos.x, aPos.y, aPos.z, 1.0);
		}
		)")
	{

	}
};

/**
 * Tests that an InstantiationException is thrown on failure to create/compile a shader
 */
BOOST_AUTO_TEST_CASE(IShader_compilationFailure)
{
    BOOST_REQUIRE_THROW(InvalidShader("InvalidShader1"), InstantiationException);
}