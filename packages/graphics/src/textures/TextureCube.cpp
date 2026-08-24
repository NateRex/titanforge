#include <graphics/textures/TextureCube.h>
#include <graphics/loaders/ImageLoader.h>
#include <common/exceptions/InstantiationException.h>
#include <common/exceptions/IllegalArgumentException.h>
#include <common/Utils.h>
#include <glad/glad.h>

TextureCube::TextureCube(const std::array<std::string, 6>& paths, bool flip): Texture(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BINDING_CUBE_MAP)
{
	int size = 0, channels = 0;
	std::array<void*, 6> images{};

	// Create lambda for releasing images
	auto release = [&images]() { for (void* data : images) stbi_image_free(data); };
	
	stbi_set_flip_vertically_on_load(flip);
	for (std::size_t i = 0; i < paths.size(); ++i)
	{
		// Load image
		int width, height, faceChannels;
		unsigned char* data = stbi_load(resolvePath(paths[i]).c_str(), &width, &height, &faceChannels, 0);
		if (!data)
		{
			release();
			throw InstantiationException("Failed to load texture image: " + paths[i]);
		}

		images[i] = data;

		// Assert width and height have equal dimensions
		if (width != height)
		{
			release();
			throw InstantiationException("Cube texture faces must be square: " + paths[i]);
		}

		// Assert dimensions match previous images
		if (i == 0)
		{
			size = width;
			channels = faceChannels;
		}
		else if (width != size || faceChannels != channels)
		{
			release();
			throw InstantiationException("Cube texture faces must have matching dimensions and channel counts: " + paths[i]);
		}
	}
	
	// Update config
	_config.size = size;
	switch (channels)
	{
		case 1: _config.format = PixelFormat::R8; break;
		case 2: _config.format = PixelFormat::RG8; break;
		case 3: _config.format = PixelFormat::RGB8; break;
		case 4: _config.format = PixelFormat::RGBA8; break;
		default: release(); throw InstantiationException("Unsupported texture channel count: " + std::to_string(channels));
	}

	allocate(images);
	release();
}

TextureCube::TextureCube(const TextureCubeConfig& config, const std::array<void*, 6>& data): Texture(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BINDING_CUBE_MAP),  _config(config)
{
	if (_config.size == 0) throw IllegalArgumentException("Cube texture size must be greater than zero");
	_config = config;
	allocate(data);
}

TextureCubePtr TextureCube::create(const std::array<std::string, 6>& paths, bool flip)
{
	return TextureCubePtr(new TextureCube(paths, flip));
}

TextureCubePtr TextureCube::create(const TextureCubeConfig& config, const std::array<void*, 6>& data)
{
	return TextureCubePtr(new TextureCube(config, data));
}

void TextureCube::allocate(const std::array<void*, 6>& data)
{
	const OpenGLPixelFormat glFormat = toGLFormat(_config.format);
	Texture::allocate(true, [this, &glFormat, &data]() {
		for (int i = 0; i < data.size(); i++)
		{
			glTexImage2D(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + static_cast<unsigned int>(i),
				0,
				glFormat.internalFormat,
				_config.size,
				_config.size,
				0,
				glFormat.format,
				glFormat.type,
				data[i]);
		}
	});
}

void TextureCube::resize(unsigned int width, unsigned int height)
{
	if (width != height) throw IllegalArgumentException("Cube texture faces must be square");
	resize(width);
}

void TextureCube::resize(unsigned int size)
{
	if (size == 0) throw IllegalArgumentException("Cube texture size must be greater than zero");
	_config.size = size;
	allocate({});
}
