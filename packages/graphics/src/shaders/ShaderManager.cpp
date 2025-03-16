#include <graphics/shaders/ShaderManager.h>
#include <graphics/shaders/Shader.h>
#include <graphics/shaders/glsl/Basic.h>
#include <common/exceptions/IllegalArgumentException.h>
#include <glad/glad.h>
#include <sstream>

std::mutex ShaderManager::_MUTEX;
std::map<std::string, Shader> ShaderManager::_SHADERS;

void ShaderManager::create(const char* name, const char* vertexShader, const char* fragmentShader)
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
	_SHADERS.emplace(name, shader);
}

void ShaderManager::use(const char* name)
{
	auto it = _SHADERS.find(name);
	if (it == _SHADERS.end())
	{
		std::ostringstream oss;
		oss << "A shader with this name does not exist: " << name;
		throw IllegalArgumentException(oss.str());
	}

	glUseProgram(it->second._id);
}

void ShaderManager::destroy(const char* name)
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
	ShaderManager::create("tf_basic", Shaders::BASIC_VERTEX, Shaders::BASIC_FRAGMENT);

	ShaderManager::use("tf_basic");
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