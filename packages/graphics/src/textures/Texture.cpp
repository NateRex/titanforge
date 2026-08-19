#include <graphics/loaders/ImageLoader.h>
#include <graphics/textures/Texture.h>
#include <common/exceptions/InstantiationException.h>
#include <common/exceptions/IllegalArgumentException.h>
#include <common/Utils.h>
#include <glad/glad.h>

Texture::Texture(const std::string& path, bool flip)
{
	int width, height, channels;

	stbi_set_flip_vertically_on_load(flip);
	unsigned char* data = stbi_load(resolvePath(path).c_str(), &width, &height, &channels, 0);
	if (!data)
	{
		throw InstantiationException("Failed to load texture image: " + path);
	}

	_descriptor.width = width;
	_descriptor.height = height;
	_descriptor.generateMipmaps = true;
	_descriptor.sampler.minFilter = TextureFilter::LINEAR_MIPMAP_LINEAR;
	switch (channels) {
		case 1: _descriptor.format = PixelFormat::R8; break;
		case 2: _descriptor.format = PixelFormat::RG8; break;
		case 3: _descriptor.format = PixelFormat::RGB8; break;
		case 4: _descriptor.format = PixelFormat::RGBA8; break;
		default: stbi_image_free(data); throw InstantiationException("Unsupported texture channel count: " + std::to_string(channels));
	}

	glGenTextures(1, &_id);

	allocate(data);
	stbi_image_free(data);
}

Texture::Texture(const TextureDescriptor& descriptor, const void* data): _descriptor(descriptor)
{
	if (_descriptor.width == 0 || _descriptor.height == 0)
	{
		throw IllegalArgumentException("Texture dimensions must be greater than zero");
	}
	
	glGenTextures(1, &_id);
	allocate(data);
}

Texture::~Texture()
{
	glDeleteTextures(1, &_id);
	_id = 0;

}

TexturePtr Texture::create(const std::string& path, bool flip)
{
	return std::shared_ptr<Texture>(new Texture(path, flip));
}

TexturePtr Texture::create(const TextureDescriptor& descriptor, const void* data)
{
	return std::shared_ptr<Texture>(new Texture(descriptor, data));
}

void Texture::allocate(const void* data)
{
	const OpenGLPixelFormat format = toGLFormat(_descriptor.format);

	int previousTexture = 0;
	int previousAlignment = 4;

	glGetIntegerv(GL_TEXTURE_BINDING_2D, &previousTexture);
	glGetIntegerv(GL_UNPACK_ALIGNMENT, &previousAlignment);
	glBindTexture(GL_TEXTURE_2D, _id);

	applySampler();

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, format.internalFormat, _descriptor.width, _descriptor.height, 0, format.format, format.type, data);

	if (_descriptor.generateMipmaps)
	{
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, previousAlignment);
	glBindTexture(GL_TEXTURE_2D, previousTexture);
}

void Texture::applySampler() const
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, toGLFilter(_descriptor.sampler.minFilter));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, toGLFilter(_descriptor.sampler.magFilter));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, toGLWrap(_descriptor.sampler.sWrap));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, toGLWrap(_descriptor.sampler.tWrap));
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, _descriptor.sampler.borderColor);
}

void Texture::resize(unsigned int width, unsigned int height)
{
	if (width == 0 || height == 0)
	{
		throw IllegalArgumentException("Texture dimensions must be greater than zero");
	}

	_descriptor.width = width;
	_descriptor.height = height;
	allocate(nullptr);
}
