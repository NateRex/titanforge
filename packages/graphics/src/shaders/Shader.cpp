#include <graphics/shaders/Shader.h>
#include <common/exceptions/InstantiationException.h>
#include <glad/glad.h>
#include <sstream>

Shader::Shader(int type, const std::string& name, const std::string& source)
	: _id(0), _loaded(false), _type(type), _name(name), _src(source)
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

void Shader::load()
{
	// Create GLFW shader, if not having done so already
	if (_id == 0)
	{
		_id = glCreateShader(_type);
		if (_id == 0)
		{
			throw InstantiationException("Failed to construct GL shader");
		}
	}

	// Load shader source
	const char* src = _src.c_str();
	glShaderSource(_id, 1, &src, NULL);

	// Compile
	int success;
	char infoLog[512];
	glCompileShader(_id);
	glGetShaderiv(_id, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(_id, 512, NULL, infoLog);
		std::ostringstream oss;
		oss << "Shader compilation failed (" << _name << "): " << infoLog;
		throw InstantiationException(oss.str());
	}

	_loaded = true;
}

void Shader::operator=(const Shader& shader)
{
	_id = shader._id;
	_loaded = shader._loaded;
	_type = shader._type;
	_name = shader._name;
	_src = shader._src;
}