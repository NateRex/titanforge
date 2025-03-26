#include <graphics/geometry/Geometry.h>
#include <common/Assertions.h>
#include <numeric>

Geometry::Geometry()
{

}

void Geometry::setVertices(const float* vertices, unsigned int numVertices)
{
	_vertices.clear();

	unsigned int len = numVertices * 3;
	for (int i = 0; i < len; i += 3)
	{
		_vertices.push_back(Vector3(vertices[i], vertices[i + 1], vertices[i + 2]));
	}

	// If indices have not been set yet, initialize them to match the number of vertices.
	// They can always be updated later by the caller.
	if (_indices.size() == 0)
	{
		_indices.resize(numVertices);
		std::iota(_indices.begin(), _indices.end(), 0);
	}
}

void Geometry::setIndices(const unsigned int* indices, unsigned int numIndices)
{
	_indices.clear();
	_indices.assign(indices, indices + numIndices);
}

unsigned int Geometry::size() const
{
	return _indices.size();
}

void Geometry::setColors(const float* colors, unsigned int numColors)
{
	_colors.clear();
	
	unsigned int len = numColors * 4;
	for (int i = 0; i < len; i += 4)
	{
		_colors.push_back(Color::fromFloats(colors[i], colors[i + 1], colors[i + 2], colors[i + 3]));
	}
}

bool Geometry::hasColors() const
{
	return _colors.size() > 0;
}

void Geometry::setTextureCoords(const float* uvs, unsigned int numUVs)
{
	_uvs.clear();

	unsigned int len = numUVs * 2;
	for (int i = 0; i < len; i += 2)
	{
		assertInRange(uvs[i], 0.f, 1.f, true, "UV coordinate must be between 0 and 1");
		assertInRange(uvs[i + 1], 0.f, 1.f, true, "UV coordinate must be between 0 and 1");
		_uvs.push_back(Vector2(uvs[i], uvs[i + 1]));
	}
}

bool Geometry::hasTextureCoords() const
{
	return _uvs.size() > 0;
}