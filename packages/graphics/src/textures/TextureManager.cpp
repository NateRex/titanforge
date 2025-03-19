#include <graphics/textures/TextureManager.h>
#include <graphics/textures/Texture.h>
#include <common/exceptions/IllegalArgumentException.h>
#include <glad/glad.h>
#include <sstream>

std::mutex TextureManager::_MUTEX;
std::map<std::string, Texture> TextureManager::_TEXTURES;

void TextureManager::setup()
{
	std::lock_guard<std::mutex> lock(_MUTEX);

	// Currently, this method does nothing. However, this may change in the future.
}

void TextureManager::clear()
{
	std::lock_guard<std::mutex> lock(_MUTEX);

	for (auto& entry : _TEXTURES)
	{
		entry.second.destroy();
	}

	_TEXTURES.clear();
}

Texture* TextureManager::create(const std::string& name, const std::string& imagePath)
{
	std::lock_guard<std::mutex> lock(_MUTEX);

	if (_TEXTURES.find(name) != _TEXTURES.end())
	{
		std::ostringstream oss;
		oss << "A texture with this name already exists: " << name;
		throw IllegalArgumentException(oss.str());
	}

	Texture tex(name, imagePath);
	tex.create();
	auto [it, inserted] = _TEXTURES.emplace(name, tex);

	return &it->second;
}

Texture* TextureManager::get(const std::string& name)
{
	return assertExists(name);
}


void TextureManager::destroy(const std::string& name)
{
	std::lock_guard<std::mutex> lock(_MUTEX);

	Texture* tex = assertExists(name);
	tex->destroy();
	_TEXTURES.erase(name);
}

Texture* TextureManager::assertExists(const std::string& name)
{
	auto it = _TEXTURES.find(name);
	if (it == _TEXTURES.end())
	{
		std::ostringstream oss;
		oss << "No texture found with name: " << name;
		throw IllegalArgumentException(oss.str());
	}
	return &it->second;
}