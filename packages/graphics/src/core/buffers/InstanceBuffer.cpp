#include <graphics/core/buffers/InstanceBuffer.h>
#include <graphics/core/buffers/GeometryBuffer.h>
#include <math/Matrix4.h>
#include <glad/glad.h>

InstanceBuffer::InstanceBuffer(const GeometryAttributes& attributes, const float* vertexData,
	unsigned int numValues, const unsigned int* indices, unsigned int numIndices)
	: GeometryBuffer(attributes, vertexData, numValues, indices, numIndices)
{
	glGenBuffers(1, &_instanceVBO);

	// The parent constructor created and configured the complete geometry VAO,
	// VBO, and optional EBO. Extend that same VAO with the instance matrix VBO.
	bind();
	glBindBuffer(GL_ARRAY_BUFFER, _instanceVBO);

	// A GLSL mat4 occupies four vertex attribute locations, one vec4 column at
	// each location. The upload transpose makes these columns contiguous.
	for (unsigned int column = 0; column < 4; ++column)
	{
		const unsigned int location = 4 + column;
		glEnableVertexAttribArray(location);
		glVertexAttribPointer(location, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 16,
			reinterpret_cast<void*>(sizeof(float) * 4 * column));
		glVertexAttribDivisor(location, 1);
	}
}

InstanceBuffer::~InstanceBuffer()
{
	glDeleteBuffers(1, &_instanceVBO);
}

void InstanceBuffer::setTransforms(const std::vector<Matrix4>& transforms)
{
	std::vector<float> values;
	values.reserve(transforms.size() * 16);

	for (const Matrix4& transform : transforms)
	{
		// Matrix contains values in row-major order, while GLSL constructs a mat4 vertex
		// attributes from four consecutive vec4 column attributes. Transposing here makes
		// the matrix values contiguous in the uploaded data.
		const Matrix4 matrix = transform.transpose();
		values.insert(values.end(), matrix.getValues(), matrix.getValues() + 16);
	}

	glBindBuffer(GL_ARRAY_BUFFER, _instanceVBO);
	glBufferData(GL_ARRAY_BUFFER, values.size() * sizeof(float), values.data(), GL_DYNAMIC_DRAW);
}
