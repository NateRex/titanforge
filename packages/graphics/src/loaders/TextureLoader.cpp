#include <graphics/loaders/TextureLoader.h>
#include <graphics/textures/Texture2D.h>
#include <common/Utils.h>

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
	const std::string cacheKey = resolvePath(path) + (flip ? "|flip" : "|no-flip");

	auto existing = loader->_textures.find(cacheKey);
	if (existing != loader->_textures.end())
	{
		return existing->second;
	}

	TexturePtr texture = Texture2D::create(path, flip);
	loader->_textures[cacheKey] = texture;
	return texture;
}

void TextureLoader::reset()
{
	if (_INSTANCE)
	{
		_INSTANCE.reset();
	}
}
