#include <graphics/textures/Texture2D.h>
#include <graphics/loaders/ImageLoader.h>
#include <common/exceptions/InstantiationException.h>
#include <common/exceptions/IllegalArgumentException.h>
#include <common/Utils.h>
#include <glad/glad.h>

Texture2D::Texture2D(const std::string& path, bool flip): Texture(GL_TEXTURE_2D, GL_TEXTURE_BINDING_2D)
{
	// Load image
	int width, height, channels;
	stbi_set_flip_vertically_on_load(flip);
	unsigned char* data = stbi_load(resolvePath(path).c_str(), &width, &height, &channels, 0);
	if (!data) throw InstantiationException("Failed to load texture image: " + path);

	// Update config
	_config.width = width;
	_config.height = height;
	_config.generateMipmaps = true;
	_config.sampling.minFilter = TextureFilter::LINEAR_MIPMAP_LINEAR;
	switch (channels)
	{
		case 1: _config.format = PixelFormat::R8; break;
		case 2: _config.format = PixelFormat::RG8; break;
		case 3: _config.format = PixelFormat::RGB8; break;
		case 4: _config.format = PixelFormat::RGBA8; break;
		default: stbi_image_free(data); throw InstantiationException("Unsupported texture channel count: " + std::to_string(channels));
	}

	allocate(data);
	stbi_image_free(data);
}

Texture2D::Texture2D(const Texture2DConfig& config, const void* data): Texture(GL_TEXTURE_2D, GL_TEXTURE_BINDING_2D), _config(config)
{
	if (_config.width <= 0 || _config.height <= 0) throw IllegalArgumentException("Texture dimensions must be greater than zero");
	allocate(data);
}

Texture2DPtr Texture2D::create(const std::string& path, bool flip)
{
	return Texture2DPtr(new Texture2D(path, flip));
}

Texture2DPtr Texture2D::create(const Texture2DConfig& config, const void* data)
{
	return Texture2DPtr(new Texture2D(config, data));
}

void Texture2D::allocate(const void* data)
{
	const OpenGLPixelFormat glFormat = toGLFormat(_config.format);
	Texture::allocate(false, [this, &glFormat, &data]() {
		glTexImage2D(GL_TEXTURE_2D, 0, glFormat.internalFormat, _config.width, _config.height, 0, glFormat.format, glFormat.type, data);
	});
}

void Texture2D::resize(unsigned int width, unsigned int height)
{
	if (width <= 0 || height <= 0) throw IllegalArgumentException("Texture dimensions must be greater than zero");
	_config.width = width;
	_config.height = height;
	
	allocate(nullptr);
}
