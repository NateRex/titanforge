#include <graphics/shaders/Shader.h>
#include <graphics/buffers/Buffer.h>
#include <graphics/textures/Texture.h>
#include <common/Assertions.h>
#include <common/exceptions/InstantiationException.h>
#include <glad/glad.h>
#include <sstream>

Shader::Shader(const std::string& name)
	: _id(0), name(name)
{

}

void Shader::draw(const Buffer* buffer) const
{
	use();

	glBindVertexArray(buffer->_vaoId);
	glDrawElements(GL_TRIANGLES, buffer->_size, GL_UNSIGNED_INT, 0);
}

void Shader::setUniform(const char* name, unsigned int textureUnit, const Texture* texture) const
{
	assertInRange(textureUnit, 0, 15, "Texture unit must be a value between 0 and 15");
	
	use();

	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_2D, texture->_id);
	glUniform1i(glGetUniformLocation(_id, name), textureUnit);
}

unsigned int Shader::compileSource(int type, const char* source)
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
		oss << "Shader " << type << " compilation failed for " << name << ": " << infoLog;
		throw InstantiationException(oss.str());
	}

	return id;
}

void Shader::link(const char* vertexShader, const char* fragmentShader)
{
	unsigned int vId = compileSource(GL_VERTEX_SHADER, vertexShader);
	unsigned int fId = compileSource(GL_FRAGMENT_SHADER, fragmentShader);

	// Create program
	_id = glCreateProgram();
	if (_id == 0)
	{
		std::ostringstream oss;
		oss << "Failed to construct shader program: " << name;
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
		oss << "Linking failed for shader program " << name << ": " << infoLog;
		throw InstantiationException(oss.str());
	}
}

void Shader::use() const
{
	glUseProgram(_id);
}

void Shader::destroy()
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