#include <graphics/textures/TextureLoader.h>
#include <graphics/textures/Texture.h>

TextureLoader::TextureLoader()
{

}

TextureLoader::~TextureLoader()
{
	_textures.clear();
}

TextureLoader& TextureLoader::getInstance()
{
	static TextureLoader _INSTANCE;		// created only once, and will exist until program exit
	return _INSTANCE;
}

TexturePtr TextureLoader::load(const std::string& path, bool flip)
{
	auto existing = _textures.find(path);
	if (existing != _textures.end())
	{
		return existing->second;
	}

	TexturePtr texture = Texture::create(path, flip);
	_textures[path] = texture;
	return texture;
}
