#include <graphics/primitives/PPolyface.h>
#include <graphics/primitives/PrimitiveAttributes.h>
#include <geometry/Vector2.h>
#include <geometry/Vector3.h>
#include <graphics/Color.h>
#include <common/Assertions.h>

PPolyface::PPolyface(const Vector3* vertices, int numVerts, const int* indices, int numIndices, const Color* colors,
	const Vector2* texCoords)
		: Polyface(vertices, numVerts, indices, numIndices)
{
	if (colors != nullptr)
	{
		_colors.insert(_colors.end(), colors, colors + numVerts);
	}
	if (texCoords != nullptr)
	{
		_texCoords.insert(_texCoords.end(), texCoords, texCoords + numVerts);
	}
}

PPolyface::PPolyface(const Polyface& polyface, const Color* colors, const Vector2* texCoords)
	: Polyface(polyface)
{
	unsigned int numVerts = polyface.getNumPositions();
	if (colors != nullptr)
	{
		_colors.insert(_colors.end(), colors, colors + numVerts);
	}
	if (texCoords != nullptr)
	{
		_texCoords.insert(_texCoords.end(), texCoords, texCoords + numVerts);
	}
}

PPolyface::~PPolyface() = default;

PrimitiveAttributes PPolyface::getAttributes() const
{
	return PrimitiveAttributes(
		_colors.size() > 0,
		_texCoords.size() > 0
	);
}

void PPolyface::buffer(std::vector<float>& vertexData, std::vector<int>& indices) const
{
	// Vertex data
	for (int i = 0; i < _vertices.size(); i++)
	{
		Vector3 p = _vertices[i];
		vertexData.push_back(p.x);
		vertexData.push_back(p.y);
		vertexData.push_back(p.z);

		if (_colors.size() > 0)
		{
			Color c = _colors[i];
			vertexData.push_back(c.red);
			vertexData.push_back(c.green);
			vertexData.push_back(c.blue);
			vertexData.push_back(c.alpha);
		}

		if (_texCoords.size() > 0)
		{
			Vector2 tc = _texCoords[i];
			vertexData.push_back(tc.x);
			vertexData.push_back(tc.y);
		}
	}

	// Indices
	for (auto& i : _indices)
	{
		if (i != -1)
		{
			indices.push_back(i);
		}
	}
}