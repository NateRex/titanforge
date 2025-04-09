#include <graphics/textures/TextureLoader.h>
#include <graphics/textures/Texture.h>

TextureLoader::TextureLoader()
{

}

TextureLoader::~TextureLoader()
{
	for (auto& pair : _textures)
	{
		pair.second.reset();
	}

	_textures.clear();
}

TextureLoader& TextureLoader::getInstance()
{
	static TextureLoader _INSTANCE;		// created only once, and will exist until program exit
	return _INSTANCE;
}

TexturePtr TextureLoader::load(const std::string& path, bool flip)
{
	TextureLoader& loader = getInstance();

	auto existing = loader._textures.find(path);
	if (existing != loader._textures.end())
	{
		return existing->second;
	}

	TexturePtr texture = Texture::create(path, flip);
	loader._textures[path] = texture;
	return texture;
}
