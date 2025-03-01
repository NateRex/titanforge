#include <graphics/Buffer.h>
#include <glad/glad.h>

Buffer::Buffer()
{
	glGenBuffers(1, &_id);
}

void Buffer::addVertices(const float* vertices, int numVertices)
{
	glBindBuffer(GL_ARRAY_BUFFER, _id);
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(float), vertices, GL_STATIC_DRAW);
}