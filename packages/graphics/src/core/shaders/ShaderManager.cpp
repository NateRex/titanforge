#include <graphics/core/shaders/ShaderManager.h>
#include <graphics/core/shaders/Shader.h>
#include <graphics/core/shaders/glsl/Basic.h>
#include <common/exceptions/IllegalArgumentException.h>
#include <glad/glad.h>
#include <sstream>

std::mutex ShaderManager::_MUTEX;
std::unordered_map<std::string, Shader> ShaderManager::_SHADERS;

Shader* ShaderManager::create(const std::string& name, const char* vertexShader, const char* fragmentShader)
{
	std::lock_guard<std::mutex> lock(_MUTEX);

	if (_SHADERS.find(name) != _SHADERS.end())
	{
		std::ostringstream oss;
		oss << "A shader with this name already exists: " << name;
		throw IllegalArgumentException(oss.str());
	}

	Shader shader(name);
	shader.link(vertexShader, fragmentShader);
	auto [it, inserted] = _SHADERS.emplace(name, shader);
	return &it->second;
}

Shader* ShaderManager::get(const std::string& name)
{
	auto it = _SHADERS.find(name);
	if (it == _SHADERS.end())
	{
		std::ostringstream oss;
		oss << "A shader with this name does not exist: " << name;
		throw IllegalArgumentException(oss.str());
	}

	return &it->second;
}

void ShaderManager::destroy(const std::string& name)
{
	std::lock_guard<std::mutex> lock(_MUTEX);

	auto it = _SHADERS.find(name);
	if (it == _SHADERS.end())
	{
		std::ostringstream oss;
		oss << "A shader with this name does not exist: " << name;
		throw IllegalArgumentException(oss.str());
	}

	it->second.destroy();
	_SHADERS.erase(name);
}

void ShaderManager::setup()
{
	Shader* basic = ShaderManager::create("tf_basic", Shaders::BASIC_VERTEX, Shaders::BASIC_FRAGMENT);
	
	basic->use();
}

void ShaderManager::clear()
{
	std::lock_guard<std::mutex> lock(_MUTEX);
	glUseProgram(0);

	for (auto& entry : _SHADERS)
	{
		entry.second.destroy();
	}

	_SHADERS.clear();
}