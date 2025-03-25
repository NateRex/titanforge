#include <graphics/objects/Mesh.h>

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

Mesh::Mesh(const float* vertices, int verticesSize, const VertexAttributes& vertexAttributes)
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

Mesh::Mesh(const float* vertices, int verticesSize, const int* indices, int indicesSize,
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

Mesh::Mesh(const Mesh& mesh)
	: Mesh(mesh._vertices, mesh._verticesSize, mesh._indices, mesh._indicesSize, mesh._vertexAttributes)
{

}

Mesh::~Mesh()
{
	delete[] _vertices;
	delete[] _indices;

	_vertices = nullptr;
	_indices = nullptr;
}

int Mesh::getSize() const
{
	return _indicesSize;
}

const VertexAttributes Mesh::getVertexAttributes() const
{
	return _vertexAttributes;
}

void Mesh::buffer(std::vector<float>& vertexData, std::vector<int>& indices) const
{
	vertexData.insert(vertexData.end(), _vertices, _vertices + _verticesSize);
	indices.insert(indices.end(), _indices, _indices + _indicesSize);
}