#include <graphics/buffers/Buffer.h>
#include <glad/glad.h>

Buffer::Buffer(const std::string& name)
	: name(name), _vaoId(0), _vboId(0), _size(0)
{
	
}

void Buffer::create(const float* data, unsigned int size)
{
	glGenVertexArrays(1, &_vaoId);
	glGenBuffers(1, &_vboId);

	glBindVertexArray(_vaoId);
	glBindBuffer(GL_ARRAY_BUFFER, _vboId);

	glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), data, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	_size = size;
}

void Buffer::destroy()
{
	GLint boundVAO = 0;
	glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &boundVAO);
	if (boundVAO == _vaoId)
	{
		// Buffer is currently bound. Make sure to unbind it first.
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	glDeleteVertexArrays(1, &_vaoId);
	glDeleteBuffers(1, &_vboId);

	_vaoId = 0;
	_vboId = 0;
}