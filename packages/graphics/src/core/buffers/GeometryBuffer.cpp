#include <graphics/core/buffers/GeometryBuffer.h>
#include <graphics/geometry/GeometryAttributes.h>
#include <glad/glad.h>

GeometryBuffer::GeometryBuffer(const GeometryAttributes& attributes, const float* vertices, unsigned int numValues,
	const unsigned int* indices, unsigned int numIndices) : _attributes(attributes)
{
	const bool hasIndices = indices != nullptr;
	const int stride = _attributes.getStride();
	_size = hasIndices ? numIndices : numValues / stride;

	// Create buffers
	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &_geometryVBO);
	if (hasIndices) glGenBuffers(1, &_geometryEBO);
	glBindVertexArray(_vao);

	// Load vertex data
	glBindBuffer(GL_ARRAY_BUFFER, _geometryVBO);
	glBufferData(GL_ARRAY_BUFFER, numValues * sizeof(float), vertices, GL_STATIC_DRAW);

	// Load index data. The EBO binding is captured by the currently bound VAO.
	if (hasIndices)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _geometryEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(unsigned int), indices, GL_STATIC_DRAW);
	}

	long long offset = 0;

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)offset);
	glEnableVertexAttribArray(0);
	offset += 3;

	// Normal attribute (if present)
	if (_attributes.normals)
	{
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(offset * sizeof(float)));
		glEnableVertexAttribArray(1);
		offset += 3;
	}

	// Color attribute (if present)
	if (_attributes.colors)
	{
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(offset * sizeof(float)));
		glEnableVertexAttribArray(2);
		offset += 4;
	}

	// Texture attribute (if present)
	if (_attributes.uvs)
	{
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(offset * sizeof(float)));
		glEnableVertexAttribArray(3);
		offset += 2;
	}
}

GeometryBuffer::~GeometryBuffer()
{
	GLint boundVAO = 0;
	glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &boundVAO);
	if (boundVAO == _vao)
	{
		// Buffer is currently bound. Make sure to unbind it first.
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	glDeleteVertexArrays(1, &_vao);
	glDeleteBuffers(1, &_geometryVBO);
	if (_geometryEBO != 0) glDeleteBuffers(1, &_geometryEBO);

	_vao = 0;
	_geometryVBO = 0;
	_geometryEBO = 0;
}

void GeometryBuffer::bind() const
{
	glBindVertexArray(_vao);
}