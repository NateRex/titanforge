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