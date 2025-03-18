#include <graphics/buffers/Buffer.h>
#include <graphics/primitives/PrimitiveAttributes.h>
#include <glad/glad.h>

Buffer::Buffer(const std::string& name)
	: name(name), _vboId(0), _eboId(0), _vaoId(0), _size(0)
{
	
}

unsigned int Buffer::computeStride(const PrimitiveAttributes& attributes)
{
	unsigned int stride = 3;

	if (attributes.hasColor)
	{
		stride += 4;
	}
	if (attributes.hasTextureCoords)
	{
		stride += 2;
	}

	return stride;
}

void Buffer::create(const PrimitiveAttributes& attributes, const float* vertices, unsigned int numVerts,
		const int* indices, unsigned int numIndices)
{
	// Create buffers
	glGenVertexArrays(1, &_vaoId);
	glGenBuffers(1, &_vboId);
	glGenBuffers(1, &_eboId);
	glBindVertexArray(_vaoId);

	// Load vertex data
	glBindBuffer(GL_ARRAY_BUFFER, _vboId);
	glBufferData(GL_ARRAY_BUFFER, numVerts * sizeof(float), vertices, GL_STATIC_DRAW);

	// Load index data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _eboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(int), indices, GL_STATIC_DRAW);
	
	unsigned int attribIdx = 0;
	unsigned int stride = computeStride(attributes);
	long long offset = 0;

	// Position attribute
	glVertexAttribPointer(attribIdx++, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*) offset);
	glEnableVertexAttribArray(0);
	offset += 3;

	// Color attribute (if present)
	if (attributes.hasColor)
	{
		glVertexAttribPointer(attribIdx++, 4, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(offset * sizeof(float)));
		glEnableVertexAttribArray(1);
		offset += 4;
	}

	// Texture attribute (if present)
	if (attributes.hasTextureCoords)
	{
		glVertexAttribPointer(attribIdx++, 2, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(offset * sizeof(float)));
		glEnableVertexAttribArray(2);
		offset += 2;
	}

	_size = numIndices;
}

void Buffer::destroy()
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