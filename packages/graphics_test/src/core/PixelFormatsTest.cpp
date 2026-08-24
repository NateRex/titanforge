#include <boost/test/unit_test.hpp>
#include <common/exceptions/IllegalArgumentException.h>
#include <graphics/core/PixelFormats.h>
#include <glad/glad.h>

/**
 * Tests conversion of each pixel format to its OpenGL representation
 */
BOOST_AUTO_TEST_CASE(PixelFormat_toGLFormat)
{
    auto checkGLFormat = [](PixelFormat format, int expectedInternalFormat, unsigned int expectedFormat, unsigned int expectedType) {
        const OpenGLPixelFormat glFormat = toGLFormat(format);
        BOOST_TEST(glFormat.internalFormat == expectedInternalFormat);
        BOOST_TEST(glFormat.format == expectedFormat);
        BOOST_TEST(glFormat.type == expectedType);
    };

	checkGLFormat(PixelFormat::R8, GL_R8, GL_RED, GL_UNSIGNED_BYTE);
	checkGLFormat(PixelFormat::RG8, GL_RG8, GL_RG, GL_UNSIGNED_BYTE);
	checkGLFormat(PixelFormat::RGB8, GL_RGB8, GL_RGB, GL_UNSIGNED_BYTE);
	checkGLFormat(PixelFormat::RGBA8, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE);
	checkGLFormat(PixelFormat::SRGB8, GL_SRGB8, GL_RGB, GL_UNSIGNED_BYTE);
	checkGLFormat(PixelFormat::SRGBA8, GL_SRGB8_ALPHA8, GL_RGBA, GL_UNSIGNED_BYTE);
	checkGLFormat(PixelFormat::RGB16F, GL_RGB16F, GL_RGB, GL_FLOAT);
	checkGLFormat(PixelFormat::RGBA16F, GL_RGBA16F, GL_RGBA, GL_FLOAT);
	checkGLFormat(PixelFormat::R32UI, GL_R32UI, GL_RED_INTEGER, GL_UNSIGNED_INT);
	checkGLFormat(PixelFormat::DEPTH24_STENCIL8, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8);
	checkGLFormat(PixelFormat::DEPTH32F, GL_DEPTH_COMPONENT32F, GL_DEPTH_COMPONENT, GL_FLOAT);
}

/**
 * Tests rejection of an unknown pixel format
 */
BOOST_AUTO_TEST_CASE(PixelFormat_toGLFormatUnknown)
{
	BOOST_REQUIRE_THROW(toGLFormat(static_cast<PixelFormat>(-1)), IllegalArgumentException);
}

/**
 * Tests identification of depth and non-depth pixel formats
 */
BOOST_AUTO_TEST_CASE(PixelFormat_isDepthFormat)
{
	BOOST_TEST(!isDepthFormat(PixelFormat::R8));
	BOOST_TEST(isDepthFormat(PixelFormat::DEPTH24_STENCIL8));
	BOOST_TEST(isDepthFormat(PixelFormat::DEPTH32F));
}
