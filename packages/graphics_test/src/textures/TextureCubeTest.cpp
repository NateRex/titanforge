#include <boost/test/unit_test.hpp>
#include <graphics/textures/TextureCube.h>
#include <common/PrintHelpers.h>
#include <common/exceptions/IllegalArgumentException.h>
#include <glad/glad.h>

/**
 * Helper method that determines the value for an OpenGL parameter of a cube map texture
 * @param texture Texture
 * @param face Face index
 * @param parameter OpenGL parameter
 * @return Integer value for that parameter on the texture
 */
int cubeFaceParameter(const TextureCube& texture, unsigned int face, unsigned int parameter)
{
    int previous = 0;
    glGetIntegerv(GL_TEXTURE_BINDING_CUBE_MAP, &previous);

    glBindTexture(GL_TEXTURE_CUBE_MAP, texture.id());
    int value = 0;
    glGetTexLevelParameteriv(face, 0, parameter, &value);

    glBindTexture(GL_TEXTURE_CUBE_MAP, previous);
    return value;
}

/**
 * Tests the cube-specific defaults applied by the storage configuration.
 */
BOOST_AUTO_TEST_CASE(TextureCube_configDefaults)
{
	TextureCubeConfig config;

	BOOST_TEST(config.size == 1);
	BOOST_TEST(config.sampling.sWrap == TextureWrap::CLAMP_TO_EDGE);
	BOOST_TEST(config.sampling.tWrap == TextureWrap::CLAMP_TO_EDGE);
	BOOST_TEST(config.sampling.rWrap == TextureWrap::CLAMP_TO_EDGE);
}

/**
 * Tests creation from a custom storage and sampler configuration.
 */
BOOST_AUTO_TEST_CASE(TextureCube_fromConfig)
{
	TextureCubeConfig config;
	config.size = 8;
	config.format = PixelFormat::RGB8;
	config.sampling.minFilter = TextureFilter::NEAREST;
	config.sampling.magFilter = TextureFilter::NEAREST;
	config.sampling.sWrap = TextureWrap::MIRRORED_REPEAT;
	config.sampling.tWrap = TextureWrap::REPEAT;
	config.generateMipmaps = true;

	TextureCubePtr texture = TextureCube::create(config);
	BOOST_REQUIRE(texture != nullptr);
	BOOST_TEST(texture->id() != 0);
	BOOST_TEST(texture->width() == config.size);
	BOOST_TEST(texture->height() == config.size);
	BOOST_TEST(texture->size() == config.size);
	BOOST_TEST(texture->format() == config.format);
	BOOST_TEST(texture->config().sampling.minFilter == config.sampling.minFilter);
	BOOST_TEST(texture->config().sampling.magFilter == config.sampling.magFilter);
	BOOST_TEST(texture->config().sampling.sWrap == config.sampling.sWrap);
	BOOST_TEST(texture->config().sampling.tWrap == config.sampling.tWrap);
	BOOST_TEST(texture->config().generateMipmaps == config.generateMipmaps);

	for (unsigned int i = 0; i < 6; ++i)
	{
		const unsigned int face = GL_TEXTURE_CUBE_MAP_POSITIVE_X + i;
		BOOST_TEST(cubeFaceParameter(*texture, face, GL_TEXTURE_WIDTH) == 8);
		BOOST_TEST(cubeFaceParameter(*texture, face, GL_TEXTURE_HEIGHT) == 8);
		BOOST_TEST(cubeFaceParameter(*texture, face, GL_TEXTURE_INTERNAL_FORMAT) == GL_RGB8);
	}
}

/**
 * Tests both cube resize overloads and preservation of the texture configuration.
 */
BOOST_AUTO_TEST_CASE(TextureCube_resize)
{
	TextureCubeConfig config;
	config.size = 2;
	config.format = PixelFormat::RGBA8;
	config.sampling.sWrap = TextureWrap::MIRRORED_REPEAT;

	TextureCubePtr texture = TextureCube::create(config);
	texture->resize(7);

	BOOST_TEST(texture->width() == 7);
	BOOST_TEST(texture->height() == 7);
	BOOST_TEST(texture->size() == 7);
	BOOST_TEST(texture->format() == config.format);
	BOOST_TEST(texture->config().sampling.sWrap == config.sampling.sWrap);

	texture->resize(5, 5);
	BOOST_TEST(texture->size() == 5);
	for (unsigned int i = 0; i < 6; ++i)
	{
		const unsigned int face = GL_TEXTURE_CUBE_MAP_POSITIVE_X + i;
		BOOST_TEST(cubeFaceParameter(*texture, face, GL_TEXTURE_WIDTH) == 5);
		BOOST_TEST(cubeFaceParameter(*texture, face, GL_TEXTURE_HEIGHT) == 5);
	}
}

/**
 * Tests validation of cube dimensions during creation and resize.
 */
BOOST_AUTO_TEST_CASE(TextureCube_invalidDimensions)
{
	TextureCubeConfig config;
	config.size = 0;
	BOOST_CHECK_THROW(TextureCube::create(config), IllegalArgumentException);

	config.size = 1;
	TextureCubePtr texture = TextureCube::create(config);
	BOOST_CHECK_THROW(texture->resize(0), IllegalArgumentException);
	BOOST_CHECK_THROW(texture->resize(2, 3), IllegalArgumentException);
	BOOST_CHECK_THROW(texture->resize(0, 0), IllegalArgumentException);
	BOOST_TEST(texture->size() == 1);
}