#include <graphics/shaders/ShaderManager.h>
#include <graphics/shaders/Shader.h>
#include <common/exceptions/IllegalArgumentException.h>
#include <sstream>

std::mutex ShaderManager::_MUTEX;
std::map<std::string, Shader> ShaderManager::_SHADERS;

void ShaderManager::loadShader(Shader& shader)
{
	std::lock_guard<std::mutex> lock(_MUTEX);

	std::string name = shader.getName();
	if (ShaderManager::_SHADERS.find(name) != ShaderManager::_SHADERS.end())
	{
		std::ostringstream oss;
		oss << "A shader with this name has already been registerd: " << name;
		throw IllegalArgumentException(oss.str());
	}

	shader.load();
	ShaderManager::_SHADERS.emplace(name, shader);
}

const Shader* ShaderManager::get(const std::string name)
{
	auto it = ShaderManager::_SHADERS.find(name);
	if (it != ShaderManager::_SHADERS.end())
	{
		return &it->second;
	}
	return nullptr;
}

void ShaderManager::clear()
{
	std::lock_guard<std::mutex> lock(_MUTEX);
	ShaderManager::_SHADERS.clear();
}