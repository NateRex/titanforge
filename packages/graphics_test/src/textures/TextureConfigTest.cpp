#include <boost/test/unit_test.hpp>
#include <graphics/textures/TextureConfig.h>
#include <glad/glad.h>

/**
 * Tests conversion of each texture filter to its OpenGL representation
 */
BOOST_AUTO_TEST_CASE(TextureConfig_toGLFilter)
{
	BOOST_TEST(toGLFilter(TextureFilter::NEAREST) == GL_NEAREST);
	BOOST_TEST(toGLFilter(TextureFilter::LINEAR) == GL_LINEAR);
	BOOST_TEST(toGLFilter(TextureFilter::NEAREST_MIPMAP_NEAREST) == GL_NEAREST_MIPMAP_NEAREST);
	BOOST_TEST(toGLFilter(TextureFilter::LINEAR_MIPMAP_LINEAR) == GL_LINEAR_MIPMAP_LINEAR);
}

/**
 * Tests that an unknown texture filter falls back to linear filtering
 */
BOOST_AUTO_TEST_CASE(TextureConfig_toGLFilterUnknown)
{
	BOOST_TEST(toGLFilter(static_cast<TextureFilter>(-1)) == GL_LINEAR);
}

/**
 * Tests conversion of each texture wrap mode to its OpenGL representation
 */
BOOST_AUTO_TEST_CASE(TextureConfig_toGLWrap)
{
	BOOST_TEST(toGLWrap(TextureWrap::REPEAT) == GL_REPEAT);
	BOOST_TEST(toGLWrap(TextureWrap::MIRRORED_REPEAT) == GL_MIRRORED_REPEAT);
	BOOST_TEST(toGLWrap(TextureWrap::CLAMP_TO_EDGE) == GL_CLAMP_TO_EDGE);
	BOOST_TEST(toGLWrap(TextureWrap::CLAMP_TO_BORDER) == GL_CLAMP_TO_BORDER);
}

/**
 * Tests that an unknown texture wrap mode falls back to repeating
 */
BOOST_AUTO_TEST_CASE(TextureConfig_toGLWrapUnknown)
{
	BOOST_TEST(toGLWrap(static_cast<TextureWrap>(-1)) == GL_REPEAT);
}
