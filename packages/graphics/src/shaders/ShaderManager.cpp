#include <graphics/shaders/programs/Basic.h>
#include <common/exceptions/IllegalArgumentException.h>
#include <common/exceptions/InstantiationException.h>
#include <sstream>

std::mutex ShaderManager::_MUTEX;
std::map<std::string, Shader> ShaderManager::_SHADERS;
std::map<std::string, ShaderProgram> ShaderManager::_PROGRAMS;
std::string ShaderManager::_ACTIVE = "";

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
		auto it = _SHADERS.find(shader);
		if (it == _SHADERS.end())
		{
			prgm._id = 0;

			std::ostringstream oss;
			oss << "Shader referenced by shader program was not mounted: " << shader;
			throw IllegalArgumentException(oss.str());
		}

		glAttachShader(prgm._id, it->second._id);
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
	if (name == _ACTIVE)
	{
		return;
	}

	ShaderProgram prgm = assertProgramExists(name);
	glUseProgram(prgm._id);
	_ACTIVE = prgm._name;
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

ShaderProgram ShaderManager::assertProgramExists(const std::string& programName)
{
	auto it = _PROGRAMS.find(programName);
	if (it == _PROGRAMS.end())
	{
		std::ostringstream oss;
		oss << "No shader program found with name: " << programName;
		throw IllegalArgumentException(oss.str());
	}

	return it->second;
}

int ShaderManager::getUniformLocation(const std::string& programName, const std::string& variableName)
{
	ShaderProgram prgm = assertProgramExists(programName);
	GLint loc = glGetUniformLocation(prgm._id, variableName.c_str());
	if (loc < 0)
	{
		std::ostringstream oss;
		oss << "Could not find uniform " << variableName << " in program " << programName;
		throw IllegalArgumentException(oss.str());
	}

	return loc;
}

void ShaderManager::setup()
{
	// Mount shaders
	ShaderManager::mountShader(basic_shader_vertex);
	ShaderManager::mountShader(basic_shader_fragment);

	// Link programs
	ShaderManager::linkProgram(basic_shader_program);

	// Unmount shaders
	ShaderManager::unmountShaders();

	// Set default shader program
	ShaderManager::useProgram(basic_shader_program._name);
}