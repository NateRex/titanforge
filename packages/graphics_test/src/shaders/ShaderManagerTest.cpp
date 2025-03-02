#include <boost/test/unit_test.hpp>
#include <graphics/Engine.h>
#include <graphics/shaders/IShader.h>
#include <graphics/shaders/VertexShader.h>
#include <graphics/shaders/ShaderManager.h>
#include <common/exceptions/IllegalArgumentException.h>
#include <glad/glad.h>

/**
 * Custom shader used for testing
 */
class TestShader : public IShader
{
public:

	/**
	 * Constructor
	 * @param name Unique name to give to this shader
	 */
	TestShader(const std::string& name) : IShader(GL_VERTEX_SHADER, name, R"(
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
	BOOST_TEST(ShaderManager::get("test1") == nullptr);

	ShaderManager::registerShader(TestShader("test1"));
	BOOST_TEST(ShaderManager::get("test1") != nullptr);
}

/**
 * Tests that an exception is thrown if we try to register a shader with the same name
 */
BOOST_AUTO_TEST_CASE(ShaderManager_cannotRegisterTwice)
{
	TestShader shader("test2");
	BOOST_REQUIRE_NO_THROW(ShaderManager::registerShader(shader));
	BOOST_REQUIRE_THROW(ShaderManager::registerShader(shader), IllegalArgumentException);
}