#include <graphics/shaders/ShaderManager.h>
#include <graphics/shaders/Shader.h>
#include <graphics/shaders/ShaderProgram.h>
#include <common/exceptions/IllegalArgumentException.h>
#include <common/exceptions/InstantiationException.h>
#include <glad/glad.h>
#include <sstream>

std::mutex ShaderManager::_MUTEX;
std::map<std::string, Shader> ShaderManager::_SHADERS;
std::map<std::string, ShaderProgram> ShaderManager::_PROGRAMS;

void ShaderManager::mountShader(Shader& shader)
{
	std::lock_guard<std::mutex> lock(_MUTEX);

	if (_SHADERS.find(shader._name) != _SHADERS.end())
	{
		std::ostringstream oss;
		oss << "A shader with this name has already been mounted: " << shader._name;
		throw IllegalArgumentException(oss.str());
	}

	shader.mount();
	_SHADERS.emplace(shader._name, shader);
}

void ShaderManager::unmountShaders()
{
	std::lock_guard<std::mutex> lock(_MUTEX);

	for (auto& entry : _SHADERS)
	{
		entry.second.unmount();
	}

	_SHADERS.clear();
}

void ShaderManager::linkProgram(ShaderProgram& prgm)
{
	std::lock_guard<std::mutex> lock(_MUTEX);

	// Assert program name does not already exist
	if (_PROGRAMS.find(prgm._name) != _PROGRAMS.end())
	{
		std::ostringstream oss;
		oss << "A shader program with this name has already been linked: " << prgm._name;
		throw IllegalArgumentException(oss.str());
	}

	// Create program
	prgm._id = glCreateProgram();
	if (prgm._id == 0)
	{
		std::ostringstream oss;
		oss << "Failed to construct GL shader program: " << prgm._name;
		throw InstantiationException(oss.str());
	}

	// Attach all shaders
	for (const auto& shader : prgm._shaders)
	{
		// Assert shader has been mounted
		if (_SHADERS.find(shader) == _SHADERS.end())
		{
			std::ostringstream oss;
			oss << "Shader referenced by shader program was not mounted: " << shader;
			throw IllegalArgumentException(oss.str());
		}

		glAttachShader(prgm._id, _SHADERS[shader]._id);
	}

	// Link program
	glLinkProgram(prgm._id);

	// Check for errors
	int success;
	char infoLog[512];
	glGetProgramiv(prgm._id, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(prgm._id, 512, NULL, infoLog);
		glDeleteProgram(prgm._id);
		prgm._id = 0;

		std::ostringstream oss;
		oss << "Shader program linking failed (" << prgm._name << "): " << infoLog;
		throw InstantiationException(oss.str());
	}

	_PROGRAMS.emplace(prgm._name, prgm);
}

void ShaderManager::useProgram(const std::string& name)
{
	if (_PROGRAMS.find(name) == _PROGRAMS.end())
	{
		std::ostringstream oss;
		oss << "No shader program found with name: " << name;
		throw IllegalArgumentException(oss.str());
	}

	glUseProgram(_PROGRAMS[name]._id);
}

void ShaderManager::clear()
{
	unmountShaders();
	glUseProgram(0);

	std::lock_guard<std::mutex> lock(_MUTEX);

	for (auto& entry : _PROGRAMS)
	{
		glDeleteProgram(entry.second._id);
	}

	_PROGRAMS.clear();
}