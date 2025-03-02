#include <graphics/shaders/IShader.h>
#include <common/exceptions/InstantiationException.h>
#include <glad/glad.h>
#include <sstream>

IShader::IShader(unsigned int type, const std::string& name, const std::string& source)
	: _type(type), _name(name), _src(source)
{
	// Create GLFW shader
	_id = glCreateShader(type);

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
}

const std::string IShader::getName() const
{
	return _name;
}

const std::string IShader::getSrc() const
{
	return _src;
}

void IShader::operator=(const IShader& shader)
{
	_name = shader._name;
	_src = shader._src;
}