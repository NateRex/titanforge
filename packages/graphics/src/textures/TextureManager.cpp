#include <graphics/textures/TextureManager.h>
#include <graphics/textures/Texture.h>
#include <common/exceptions/IllegalArgumentException.h>
#include <common/exceptions/NullPointerException.h>
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

Texture TextureManager::create(const std::string& name, const std::string& relPath)
{
	std::lock_guard<std::mutex> lock(_MUTEX);

	if (_TEXTURES.find(name) != _TEXTURES.end())
	{
		std::ostringstream oss;
		oss << "A texture with this name already exists: " << name;
		throw IllegalArgumentException(oss.str());
	}

	Texture tex(name, relPath);
	tex.create();
	_TEXTURES.emplace(name, tex);

	return tex;
}

Texture TextureManager::get(const std::string& name)
{
	auto it = _TEXTURES.find(name);
	if (it == _TEXTURES.end())
	{
		std::ostringstream oss;
		oss << "A texture with this name does not exist: " << name;
		throw NullPointerException(oss.str());
	}

	return it->second;
}

void TextureManager::destroy(const std::string& name)
{
	std::lock_guard<std::mutex> lock(_MUTEX);

	auto it = _TEXTURES.find(name);
	if (it == _TEXTURES.end())
	{
		std::ostringstream oss;
		oss << "No texture found with name: " << name;
		throw IllegalArgumentException(oss.str());
	}

	it->second.destroy();
	_TEXTURES.erase(name);
}