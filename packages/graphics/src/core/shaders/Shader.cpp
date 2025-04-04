#include <graphics/core/shaders/Shader.h>
#include <math/Matrix4.h>
#include <common/exceptions/IllegalArgumentException.h>
#include <common/exceptions/InstantiationException.h>
#include <glad/glad.h>
#include <sstream>

Shader::Shader(const char* prgmName, const char* vertexShader, const char* fragmentShader)
{
	unsigned int vId = compileSource(prgmName, GL_VERTEX_SHADER, vertexShader);
	unsigned int fId = compileSource(prgmName, GL_FRAGMENT_SHADER, fragmentShader);

	// Create program
	_id = glCreateProgram();
	if (_id == 0)
	{
		std::ostringstream oss;
		oss << "Failed to construct shader program: " << prgmName;
		throw InstantiationException(oss.str());
	}

	// Attach shaders and link
	glAttachShader(_id, vId);
	glAttachShader(_id, fId);
	glLinkProgram(_id);

	// Detach shaders
	glDeleteShader(vId);
	glDeleteShader(fId);

	// Check for errors
	int success;
	char infoLog[512];
	glGetProgramiv(_id, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(_id, 512, NULL, infoLog);
		glDeleteProgram(_id);

		_id = 0;

		std::ostringstream oss;
		oss << "Linking failed for shader program " << prgmName << ": " << infoLog;
		throw InstantiationException(oss.str());
	}
}

Shader::~Shader()
{
	GLint boundProgram = 0;
	glGetIntegerv(GL_CURRENT_PROGRAM, &boundProgram);
	if (boundProgram == _id)
	{
		// Program is currently in use. Make sure to unbind it first.
		glUseProgram(0);
	}

	glDeleteProgram(_id);
	_id = 0;
}

unsigned int Shader::compileSource(const char* prgmName, int type, const char* source)
{
	unsigned int id = glCreateShader(type);
	if (id == 0)
	{
		throw InstantiationException("Failed to construct shader");
	}

	// Load shader source
	glShaderSource(id, 1, &source, NULL);

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
		oss << "Shader " << type << " compilation failed for " << prgmName << ": " << infoLog;
		throw InstantiationException(oss.str());
	}

	return id;
}

void Shader::use() const
{
	glUseProgram(_id);
}

int Shader::getUniformLocation(const char* variableName) const
{
	int loc = glGetUniformLocation(_id, variableName);
	if (loc < 0)
	{
		std::ostringstream oss;
		oss << "Could not find uniform variable: " << variableName;
		throw IllegalArgumentException(oss.str());
	}

	return loc;
}

void Shader::setModelMatrix(const Matrix4& matrix)
{
	int loc = getUniformLocation("model");
	glUniformMatrix4fv(loc, 1, GL_TRUE, matrix.getValues());
}

void Shader::setViewMatrix(const Matrix4& matrix)
{
	int loc = getUniformLocation("view");
	glUniformMatrix4fv(loc, 1, GL_TRUE, matrix.getValues());
}

void Shader::setProjectionMatrix(const Matrix4& matrix)
{
	int loc = getUniformLocation("proj");
	glUniformMatrix4fv(loc, 1, GL_TRUE, matrix.getValues());
}