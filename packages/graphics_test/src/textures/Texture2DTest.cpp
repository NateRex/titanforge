#include <boost/test/unit_test.hpp>
#include <graphics/textures/Texture2D.h>
#include <common/PrintHelpers.h>
#include <common/exceptions/InstantiationException.h>
#include <common/exceptions/IllegalArgumentException.h>
#include <glad/glad.h>

/**
 * Helper method that determines the value for an OpenGL parameter of a texture
 * @param texture Texture
 * @param parameter OpenGL parameter
 * @return Integer value for that parameter on the texture
 */
int texture2DParameter(const Texture& texture, unsigned int parameter)
{
	int previous = 0;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &previous);

	glBindTexture(GL_TEXTURE_2D, texture.id());
	int value = 0;
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, parameter, &value);

	glBindTexture(GL_TEXTURE_2D, previous);
	return value;
}

/**
 * Tests creation of a texture from an image file
 */
BOOST_AUTO_TEST_CASE(Texture2D_fromImage)
{
	TexturePtr texture = Texture2D::create("assets/container.jpg");
	BOOST_TEST(texture != nullptr);
	BOOST_TEST(texture->id() > 0);
}

/**
 * Tests that creation of a texture from an image that does not exist results in exception
 */
BOOST_AUTO_TEST_CASE(Texture2D_nonExistentImage)
{
	BOOST_REQUIRE_THROW(Texture2D::create("does-not-exist"), InstantiationException);
}

/**
 * Tests creation of a texture from a custom storage and sampler config
 */
BOOST_AUTO_TEST_CASE(Texture2D_fromConfig)
{
	Texture2DConfig config;
	config.width = 8;
	config.height = 4;
	config.format = PixelFormat::RGB8;
	config.sampling.minFilter = TextureFilter::NEAREST;
	config.sampling.magFilter = TextureFilter::NEAREST;
	config.sampling.sWrap = TextureWrap::CLAMP_TO_EDGE;
	config.sampling.tWrap = TextureWrap::MIRRORED_REPEAT;
	config.generateMipmaps = true;

	TexturePtr texture = Texture2D::create(config);
	BOOST_REQUIRE(texture != nullptr);
	BOOST_TEST(texture->id() != 0);
	BOOST_TEST(texture->width() == config.width);
	BOOST_TEST(texture->height() == config.height);
	BOOST_TEST(texture->format() == config.format);
	BOOST_TEST(texture->config().sampling.minFilter == config.sampling.minFilter);
	BOOST_TEST(texture->config().sampling.magFilter == config.sampling.magFilter);
	BOOST_TEST(texture->config().sampling.sWrap == config.sampling.sWrap);
	BOOST_TEST(texture->config().sampling.tWrap == config.sampling.tWrap);
	BOOST_TEST(texture->config().generateMipmaps == config.generateMipmaps);
	BOOST_TEST(texture2DParameter(*texture, GL_TEXTURE_WIDTH) == 8);
	BOOST_TEST(texture2DParameter(*texture, GL_TEXTURE_HEIGHT) == 4);
	BOOST_TEST(texture2DParameter(*texture, GL_TEXTURE_INTERNAL_FORMAT) == GL_RGB8);
}

/**
 * Tests resizing texture storage
 */
BOOST_AUTO_TEST_CASE(Texture2D_resize)
{
	Texture2DConfig config;
	config.width = 2;
	config.height = 3;
	config.format = PixelFormat::RGBA8;
	config.sampling.sWrap = TextureWrap::CLAMP_TO_BORDER;

	TexturePtr texture = Texture2D::create(config);

	texture->resize(7, 5);
	BOOST_TEST(texture->width() == 7);
	BOOST_TEST(texture->height() == 5);
	BOOST_TEST(texture->format() == config.format);
	BOOST_TEST(texture->config().sampling.sWrap == config.sampling.sWrap);
	BOOST_TEST(texture2DParameter(*texture, GL_TEXTURE_WIDTH) == 7);
	BOOST_TEST(texture2DParameter(*texture, GL_TEXTURE_HEIGHT) == 5);

	glBindTexture(GL_TEXTURE_2D, 0);
}

/**
 * Tests validation of texture dimensions during creation and resize
 */
BOOST_AUTO_TEST_CASE(Texture2D_invalidDimensions)
{
	Texture2DConfig config;
	config.width = 0;
	BOOST_CHECK_THROW(Texture2D::create(config), IllegalArgumentException);

	config.width = 1;
	config.height = 0;
	BOOST_CHECK_THROW(Texture2D::create(config), IllegalArgumentException);

	config.height = 1;
	TexturePtr texture = Texture2D::create(config);
	BOOST_CHECK_THROW(texture->resize(0, 1), IllegalArgumentException);
	BOOST_CHECK_THROW(texture->resize(1, 0), IllegalArgumentException);
	BOOST_TEST(texture->width() == 1);
	BOOST_TEST(texture->height() == 1);
}
