#include <graphics/shaders/Shader.h>
#include <common/exceptions/InstantiationException.h>
#include <glad/glad.h>
#include <sstream>

Shader::Shader(int type, const std::string& name, const std::string& source)
	: _id(0), _type(type), _name(name), _src(source)
{

}

const std::string Shader::getName() const
{
	return _name;
}

const std::string Shader::getSrc() const
{
	return _src;
}

void Shader::mount()
{
	if (_id != 0)
	{
		// Shader already mounted
		return;
	}

	unsigned int id = glCreateShader(_type);
	if (id == 0)
	{
		throw InstantiationException("Failed to construct GL shader");
	}

	// Load shader source
	const char* src = _src.c_str();
	glShaderSource(id, 1, &src, NULL);

	// Compile
	glCompileShader(id);

	// Check for errors
	int success;
	char infoLog[512];
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(id, 512, NULL, infoLog);
		glDeleteShader(id);

		std::ostringstream oss;
		oss << "Shader compilation failed (" << _name << "): " << infoLog;
		throw InstantiationException(oss.str());
	}

	_id = id;
}

void Shader::unmount()
{
	if (_id == 0)
	{
		// Shader not currently mounted
		return;
	}

	glDeleteShader(_id);
	_id = 0;
}