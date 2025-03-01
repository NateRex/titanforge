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

#pragma warning( push )
#pragma warning( disable : 6386)
void PPolyface::buffer(Buffer& buffer) const
{
	float* flattened = new float[_vertices.size() * 3];
	for (int i = 0; i < _vertices.size(); ++i)
	{
		int vertex = _vertices[i];
		if (vertex == -1) {
			continue;
		}

		Vector3 pos = _positions[_vertices[i]];

		int idx = i * 3;
		flattened[idx] = pos.x;
		flattened[idx + 1] = pos.y;
		flattened[idx + 2] = pos.z;
	}

	buffer.addVertices(flattened, _vertices.size() * 3);

	delete[] flattened;
}
#pragma warning( pop )