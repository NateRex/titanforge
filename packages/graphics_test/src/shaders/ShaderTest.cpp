#include <boost/test/unit_test.hpp>
#include <graphics/shaders/Shader.h>
#include <graphics/shaders/ShaderManager.h>
#include <graphics/shaders/glsl/Basic.h>
#include <common/exceptions/InstantiationException.h>
#include <glad/glad.h>

/**
 * Tests the basic constructors and accessors of a shader
 */
BOOST_AUTO_TEST_CASE(Shader_basics)
{
	Shader* shader = ShaderManager::create("shader", Shaders::BASIC_VERTEX, Shaders::BASIC_FRAGMENT);
	BOOST_TEST(shader->name == "shader");
	BOOST_REQUIRE_NO_THROW(ShaderManager::destroy("shader"));
}

/**
 * Tests that a shader can be constructed, used, and destroyed via the manager
 */
BOOST_AUTO_TEST_CASE(Shader_use)
{
	Shader* shader = ShaderManager::create("shader", Shaders::BASIC_VERTEX, Shaders::BASIC_FRAGMENT);
	BOOST_REQUIRE_NO_THROW(shader->use());
	BOOST_REQUIRE_NO_THROW(ShaderManager::destroy("shader"));
}

/**
 * Tests that an InstantiationException is thrown on failure to compile a shader
 */
BOOST_AUTO_TEST_CASE(Shader_compilationFailure)
{
	const char* invalidVertex = R"(
		#version 330 core

		layout (location = 0) in vec3 aPos;

		void main()
		{
			gl_Position = vec1000(aPos.x, aPos.y, aPos.z, 1.0);
		}
		)";
	BOOST_REQUIRE_THROW(ShaderManager::create("shader", invalidVertex, Shaders::BASIC_FRAGMENT),
			InstantiationException);
}