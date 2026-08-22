#include <graphics/core/PixelFormats.h>
#include <common/exceptions/IllegalArgumentException.h>
#include <glad/glad.h>

OpenGLPixelFormat toGLFormat(PixelFormat format)
{
	switch (format)
	{
		case PixelFormat::R8: return { GL_R8, GL_RED, GL_UNSIGNED_BYTE };
		case PixelFormat::RG8: return { GL_RG8, GL_RG, GL_UNSIGNED_BYTE };
		case PixelFormat::RGB8: return { GL_RGB8, GL_RGB, GL_UNSIGNED_BYTE };
		case PixelFormat::RGBA8: return { GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE };
		case PixelFormat::SRGB8: return { GL_SRGB8, GL_RGB, GL_UNSIGNED_BYTE };
		case PixelFormat::SRGBA8: return { GL_SRGB8_ALPHA8, GL_RGBA, GL_UNSIGNED_BYTE };
		case PixelFormat::RGB16F: return { GL_RGB16F, GL_RGB, GL_FLOAT };
		case PixelFormat::RGBA16F: return { GL_RGBA16F, GL_RGBA, GL_FLOAT };
		case PixelFormat::R32UI: return { GL_R32UI, GL_RED_INTEGER, GL_UNSIGNED_INT };
		case PixelFormat::DEPTH24_STENCIL8: return { GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8 };
		case PixelFormat::DEPTH32F: return { GL_DEPTH_COMPONENT32F, GL_DEPTH_COMPONENT, GL_FLOAT };
	}
	throw IllegalArgumentException("Unknown texture format");
}