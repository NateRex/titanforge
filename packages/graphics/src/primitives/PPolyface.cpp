#include <graphics/primitives/PPolyface.h>
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

void PPolyface::buffer(std::vector<float>& buffer) const
{
	for (int i = 0; i < _vertices.size(); ++i)
	{
		int vertex = _vertices[i];
		if (vertex == -1) {
			continue;
		}

		Vector3 pos = _positions[_vertices[i]];

		int idx = i * 3;
		buffer.push_back(pos.x);
		buffer.push_back(pos.y);
		buffer.push_back(pos.z);
	}
}