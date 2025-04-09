#include <graphics/textures/TextureLoader.h>
#include <graphics/textures/Texture.h>

std::unique_ptr<TextureLoader> TextureLoader::_INSTANCE = nullptr;

TextureLoader::TextureLoader()
{

}

TextureLoader::~TextureLoader()
{
	_textures.clear();
}

TextureLoader* TextureLoader::getInstance()
{
	if (!_INSTANCE)
	{
		_INSTANCE = std::unique_ptr<TextureLoader>(new TextureLoader());
	}

	return _INSTANCE.get();
}

TexturePtr TextureLoader::load(const std::string& path, bool flip)
{
	TextureLoader* loader = getInstance();

	auto existing = loader->_textures.find(path);
	if (existing != loader->_textures.end())
	{
		return existing->second;
	}

	TexturePtr texture = Texture::create(path, flip);
	loader->_textures[path] = texture;
	return texture;
}

void TextureLoader::reset()
{
	if (_INSTANCE)
	{
		_INSTANCE.reset();
	}
}