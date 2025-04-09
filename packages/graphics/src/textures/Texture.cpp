#include <graphics/textures/Texture.h>
#include <graphics/textures/ImageLoader.h>
#include <common/exceptions/InstantiationException.h>
#include <common/Utils.h>
#include <glad/glad.h>
#include <sstream>

Texture::Texture(const std::string& path, bool flip)
{
	// Resolve image path
	std::string fullPath = resolvePath(path);

	// Load image
	int width, height, channels;
	stbi_set_flip_vertically_on_load(flip);
	unsigned char* data = stbi_load(fullPath.c_str(), &width, &height, &channels, 0);
	if (!data)
	{
		std::ostringstream oss;
		oss << "Failed to load texture image: " << path;
		throw InstantiationException(oss.str());
	}

	// Create texture
	glGenTextures(1, &_id);
	glBindTexture(GL_TEXTURE_2D, _id);

	// Set texture wrapping and filtering options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Load image data
	unsigned int glRgb = checkSuffix(path, ".png") ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_2D, 0, glRgb, width, height, 0, glRgb, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Free the image data
	stbi_image_free(data);
}

Texture::~Texture()
{
	GLint boundId;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &boundId);
	if (boundId == _id)
	{
		// Texture is currently bound. Make sure to unbind it first.
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	glDeleteTextures(1, &_id);
	_id = 0;
}

TexturePtr Texture::create(const std::string& path, bool flip)
{
	return std::shared_ptr<Texture>(new Texture(path, flip));
}

unsigned int Texture::id() const
{
	return _id;
}