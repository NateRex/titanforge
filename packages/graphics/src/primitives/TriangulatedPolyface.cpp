#include <graphics/primitives/TriangulatedPolyface.h>
#include <algorithm>
#include <numeric>

TriangulatedPolyface::TriangulatedPolyface(const float* vertices, int verticesSize, const VertexAttributes& vertexAttributes)
		: Primitive(vertexAttributes)
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

TriangulatedPolyface::TriangulatedPolyface(const float* vertices, int verticesSize, const int* indices, int indicesSize,
	const VertexAttributes& vertexAttributes)
		: Primitive(vertexAttributes)
{
	// Copy vertices
	_verticesSize = verticesSize;
	_vertices = new float[verticesSize];
	std::copy(vertices, vertices + verticesSize, _vertices);

	_indicesSize = indicesSize;
	_indices = new int[indicesSize];
	std::copy(indices, indices + indicesSize, _indices);
}

TriangulatedPolyface::TriangulatedPolyface(const TriangulatedPolyface& polyface)
	: TriangulatedPolyface(polyface._vertices, polyface._verticesSize, polyface._indices, polyface._indicesSize, polyface._vertexAttributes)
{

}

TriangulatedPolyface::~TriangulatedPolyface()
{
	delete[] _vertices;
	delete[] _indices;

	_vertices = nullptr;
	_indices = nullptr;
}

int TriangulatedPolyface::getSize() const
{
	return _indicesSize;
}

void TriangulatedPolyface::buffer(std::vector<float>& vertexData, std::vector<int>& indices) const
{
	vertexData.insert(vertexData.end(), _vertices, _vertices + _verticesSize);
	indices.insert(indices.end(), _indices, _indices + _indicesSize);
}