#include <graphics/textures/TextureFormats.h>
#include <glad/glad.h>

unsigned int toGLFilter(const TextureFilter& filter)
{
	switch (filter)
	{
		case TextureFilter::NEAREST: return GL_NEAREST;
		case TextureFilter::LINEAR: return GL_LINEAR;
		case TextureFilter::NEAREST_MIPMAP_NEAREST: return GL_NEAREST_MIPMAP_NEAREST;
		case TextureFilter::LINEAR_MIPMAP_LINEAR: return GL_LINEAR_MIPMAP_LINEAR;
	}
	return GL_LINEAR;
}

unsigned int toGLWrap(const TextureWrap& wrap)
{
	switch (wrap)
	{
		case TextureWrap::REPEAT: return GL_REPEAT;
		case TextureWrap::MIRRORED_REPEAT: return GL_MIRRORED_REPEAT;
		case TextureWrap::CLAMP_TO_EDGE: return GL_CLAMP_TO_EDGE;
		case TextureWrap::CLAMP_TO_BORDER: return GL_CLAMP_TO_BORDER;
	}
	return GL_REPEAT;
}