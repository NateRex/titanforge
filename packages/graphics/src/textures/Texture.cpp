#include <graphics/textures/Texture.h>
#include <glad/glad.h>

Texture::Texture(unsigned int type, unsigned int binding): _type(type), _binding(binding) {
	glGenTextures(1, &_id);
}

Texture::~Texture()
{
	glDeleteTextures(1, &_id);
	_id = 0;
}

void Texture::allocate(bool includeRWrap, const std::function<void()>& upload, bool applySampling)
{
	// Record previous texture and alignment
	int previousTexture = 0, previousAlignment = 4;
	glGetIntegerv(_binding, &previousTexture);
	glGetIntegerv(GL_UNPACK_ALIGNMENT, &previousAlignment);

	glBindTexture(_type, _id);

	// Multisampled textures have fixed sampling behavior and do not support sampler parameters.
	const TextureConfig& conf = config();
	if (applySampling)
	{
		const TextureSampling& sampling = conf.sampling;
		glTexParameteri(_type, GL_TEXTURE_MIN_FILTER, toGLFilter(sampling.minFilter));
		glTexParameteri(_type, GL_TEXTURE_MAG_FILTER, toGLFilter(sampling.magFilter));
		glTexParameteri(_type, GL_TEXTURE_WRAP_S, toGLWrap(sampling.sWrap));
		glTexParameteri(_type, GL_TEXTURE_WRAP_T, toGLWrap(sampling.tWrap));
		if (includeRWrap) glTexParameteri(_type, GL_TEXTURE_WRAP_R, toGLWrap(sampling.rWrap));
		glTexParameterfv(_type, GL_TEXTURE_BORDER_COLOR, sampling.borderColor);
	}

	// Upload data
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	upload();
	if (applySampling && conf.generateMipmaps) glGenerateMipmap(_type);

	// Restore previous texture
	glPixelStorei(GL_UNPACK_ALIGNMENT, previousAlignment);
	glBindTexture(_type, previousTexture);
}
