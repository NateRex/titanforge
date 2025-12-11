#include <graphics/core/Buffer.h>
#include <graphics/geometry/GeometryAttributes.h>
#include <glad/glad.h>

Buffer::Buffer(const GeometryAttributes& attributes, const float* vertices, unsigned int numValues,
	const unsigned int* indices, unsigned int numIndices): size(numIndices)
{
	// Create buffers
	glGenVertexArrays(1, &_vaoId);
	glGenBuffers(1, &_vboId);
	glGenBuffers(1, &_eboId);
	glBindVertexArray(_vaoId);

	// Load vertex data
	glBindBuffer(GL_ARRAY_BUFFER, _vboId);
	glBufferData(GL_ARRAY_BUFFER, numValues * sizeof(float), vertices, GL_STATIC_DRAW);

	// Load index data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _eboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	int stride = attributes.getStride();
	long long offset = 0;

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)offset);
	glEnableVertexAttribArray(0);
	offset += 3;

	// Normal attribute (if present)
	if (attributes.normals)
	{
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(offset * sizeof(float)));
		glEnableVertexAttribArray(1);
		offset += 3;
	}

	// Color attribute (if present)
	if (attributes.colors)
	{
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(offset * sizeof(float)));
		glEnableVertexAttribArray(2);
		offset += 4;
	}

	// Texture attribute (if present)
	if (attributes.uvs)
	{
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(offset * sizeof(float)));
		glEnableVertexAttribArray(3);
		offset += 2;
	}
}

Buffer::~Buffer()
{
	GLint boundVAO = 0;
	glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &boundVAO);
	if (boundVAO == _vaoId)
	{
		// Buffer is currently bound. Make sure to unbind it first.
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	glDeleteVertexArrays(1, &_vaoId);
	glDeleteBuffers(1, &_vboId);
	glDeleteBuffers(1, &_eboId);

	_vaoId = 0;
	_vboId = 0;
	_eboId = 0;
}

void Buffer::bind() const
{
	glBindVertexArray(_vaoId);
}