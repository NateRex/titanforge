#include <graphics/shaders/ShaderManager.h>
#include <graphics/shaders/IShader.h>
#include <graphics/shaders/VertexShader.h>
#include <common/exceptions/IllegalArgumentException.h>
#include <sstream>

std::mutex ShaderManager::_MUTEX;
std::map<std::string, IShader> ShaderManager::_SHADERS;

void ShaderManager::registerShader(const IShader& shader)
{
	std::lock_guard<std::mutex> lock(_MUTEX);

	std::string name = shader.getName();
	if (ShaderManager::_SHADERS.find(name) != ShaderManager::_SHADERS.end())
	{
		std::ostringstream oss;
		oss << "A shader with this name has already been registerd: " << name;
		throw IllegalArgumentException(oss.str());
	}

	ShaderManager::_SHADERS.emplace(name, shader);
}

const IShader* ShaderManager::get(const std::string name)
{
	auto it = ShaderManager::_SHADERS.find(name);
	if (it != ShaderManager::_SHADERS.end())
	{
		return &it->second;
	}
	return nullptr;
}

void ShaderManager::registerDefaults()
{
	ShaderManager::registerShader(VertexShader());
}

void ShaderManager::clear()
{
	std::lock_guard<std::mutex> lock(_MUTEX);
	ShaderManager::_SHADERS.clear();
}