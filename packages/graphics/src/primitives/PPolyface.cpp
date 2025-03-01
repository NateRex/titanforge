#include <graphics/primitives/PPolyface.h>
#include <graphics/Buffer.h>
#include <geometry/Vector3.h>

PPolyface::PPolyface(const Vector3* pos, int numPos, const int* verts, int numVerts)
	: Polyface(pos, numPos, verts, numVerts)
{

}

PPolyface::PPolyface(const std::vector<Vector3>& positions, const std::vector<int> vertices)
	: Polyface(positions, vertices)
{

}

PPolyface::PPolyface(const Polyface& polyface)
	: Polyface(polyface)
{

}

void PPolyface::buffer(Buffer& buffer) const
{
	std::vector<float> flattened(_vertices.size() * 3);
	for (int i = 0; i < _vertices.size(); ++i)
	{
		Vector3 pos = _positions[_vertices[i]];
		flattened.push_back(pos.x);
		flattened.push_back(pos.y);
		flattened.push_back(pos.z);
	}

	buffer.addVertices(flattened.data(), _vertices.size());
}