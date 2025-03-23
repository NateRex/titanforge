#include <graphics/primitives/Primitive.h>

// ------------------------------------------------------------------------------------------------------------------
// VertexAttributes
// ------------------------------------------------------------------------------------------------------------------

int VertexAttributes::getStride() const
{
	int stride = 3;

	if (hasColor)
	{
		stride += 4;
	}

	if (hasTexture)
	{
		stride += 2;
	}

	return stride;
}

bool VertexAttributes::operator==(const VertexAttributes& other) const
{
	return hasColor == other.hasColor
		&& hasTexture == other.hasTexture;
}

bool VertexAttributes::operator!=(const VertexAttributes& other) const
{
	return !(*this == other);
}


// ------------------------------------------------------------------------------------------------------------------
// Polyface
// ------------------------------------------------------------------------------------------------------------------

#include <algorithm>
#include <numeric>

Primitive::Primitive(const float* vertices, int verticesSize, const VertexAttributes& vertexAttributes)
	: _vertexAttributes(vertexAttributes)
{
	// Copy vertices
	_verticesSize = verticesSize;
	_vertices = new float[verticesSize];
	std::copy(vertices, vertices + verticesSize, _vertices);

	// Create indices
	int vertexStride = vertexAttributes.getStride();
	_indicesSize = verticesSize / vertexStride;
	_indices = new int[_indicesSize];
	std::iota(_indices, _indices + _indicesSize, 0);
}

Primitive::Primitive(const float* vertices, int verticesSize, const int* indices, int indicesSize,
		const VertexAttributes& vertexAttributes)
	: _vertexAttributes(vertexAttributes)
{
	// Copy vertices
	_verticesSize = verticesSize;
	_vertices = new float[verticesSize];
	std::copy(vertices, vertices + verticesSize, _vertices);

	// Copy indices
	_indicesSize = indicesSize;
	_indices = new int[indicesSize];
	std::copy(indices, indices + indicesSize, _indices);
}

Primitive::Primitive(const Primitive& primitive)
	: Primitive(primitive._vertices, primitive._verticesSize, primitive._indices, primitive._indicesSize, primitive._vertexAttributes)
{

}

Primitive::~Primitive()
{
	delete[] _vertices;
	delete[] _indices;

	_vertices = nullptr;
	_indices = nullptr;
}

int Primitive::getSize() const
{
	return _indicesSize;
}

const VertexAttributes Primitive::getVertexAttributes() const
{
	return _vertexAttributes;
}

void Primitive::buffer(std::vector<float>& vertexData, std::vector<int>& indices) const
{
	vertexData.insert(vertexData.end(), _vertices, _vertices + _verticesSize);
	indices.insert(indices.end(), _indices, _indices + _indicesSize);
}