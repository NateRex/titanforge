#include <boost/test/unit_test.hpp>
#include <graphics_test/testutils/ShaderTestFixture.h>
#include <graphics/buffers/BufferManager.h>
#include <graphics/textures/TextureManager.h>
#include <graphics/shaders/Shader.h>
#include <graphics/shaders/ShaderManager.h>
#include <graphics/shaders/glsl/Basic.h>
#include <math/Matrix4.h>
#include <common/exceptions/IllegalArgumentException.h>
#include <common/exceptions/InstantiationException.h>
#include <glad/glad.h>

BOOST_AUTO_TEST_SUITE(ShaderTestSuite, *boost::unit_test::fixture<ShaderTestFixture>())

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
 * Tests that a buffer can be drawn using a shader
 */
BOOST_AUTO_TEST_CASE(Shader_draw)
{
	// Create buffers
	Buffer* buffer1 = BufferManager::startBuffer("buffer1").finish();
	Buffer* buffer2 = BufferManager::startBuffer("buffer2").finish();

	// At this point, buffer2 should be bound
	GLuint binding1;
	glGetIntegerv(GL_VERTEX_ARRAY_BINDING, (GLint*)&binding1);

	// Draw buffer1
	Shader* shader = ShaderManager::get("test");
	shader->draw(buffer1);

	// Verify buffer2 no longer bound
	GLuint binding2;
	glGetIntegerv(GL_VERTEX_ARRAY_BINDING, (GLint*)&binding2);
	BOOST_TEST(binding1 != binding2);

	// Destroy both buffers
	BufferManager::destroy("buffer1");
	BufferManager::destroy("buffer2");

	// Verify no buffer is bound
	GLuint binding3;
	glGetIntegerv(GL_VERTEX_ARRAY_BINDING, (GLint*)&binding3);
	BOOST_TEST(binding3 == 0);
}

/**
 * Tests the ability to set a texture uniform
 */
BOOST_AUTO_TEST_CASE(Shader_setUniformTexture)
{	
	Shader* shader = ShaderManager::get("test");

	Texture* texture = TextureManager::create("texture", "assets/container.jpg");
	BOOST_REQUIRE_NO_THROW(shader->setUniform("texture1", 0, texture));

	TextureManager::destroy("texture");
}

/**
 * Tests the ability to set a 4x4 matrix uniform
 */
BOOST_AUTO_TEST_CASE(Shader_setUniformMatrix4)
{
	Shader* shader = ShaderManager::get("test");
	BOOST_REQUIRE_NO_THROW(shader->setUniform("matrix1", Matrix4::IDENTITY));
}

/**
 * Tests that an exception is thrown trying to set a uniform variable that does not exist
 */
BOOST_AUTO_TEST_CASE(Shader_missingUniformException)
{
	Shader* shader = ShaderManager::get("test");
	BOOST_REQUIRE_THROW(shader->setUniform("does-not-exist", Matrix4::IDENTITY), IllegalArgumentException);
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

BOOST_AUTO_TEST_SUITE_END()