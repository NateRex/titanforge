#include <graphics/primitives/PPolyface.h>
#include <graphics/primitives/PrimitiveAttributes.h>
#include <geometry/Vector3.h>
#include <graphics/Color.h>

PPolyface::PPolyface(const Vector3* vertices, int numVerts, const int* indices, int numIndices, const Color* colors)
	: Polyface(vertices, numVerts, indices, numIndices)
{
	if (colors != nullptr)
	{
		_colors.insert(_colors.end(), colors, colors + numVerts);
	}
}

PPolyface::PPolyface(const std::vector<Vector3>& positions, const std::vector<int>& vertices, const std::vector<Color>& colors)
	: Polyface(positions, vertices), _colors(colors)
{

}

PPolyface::PPolyface(const Polyface& polyface)
	: Polyface(polyface)
{

}

PrimitiveAttributes PPolyface::getAttributes() const
{
	return PrimitiveAttributes(_colors.size() > 0);
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