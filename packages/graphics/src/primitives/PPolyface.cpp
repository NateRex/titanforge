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

void PPolyface::buffer(std::vector<float>& vertices, std::vector<int>& indices) const
{
	for (auto& p : _vertices)
	{
		vertices.push_back(p.x);
		vertices.push_back(p.y);
		vertices.push_back(p.z);
	}
	for (auto& i : _indices)
	{
		if (i != -1)
		{
			indices.push_back(i);
		}
	}
}