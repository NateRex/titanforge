#include <graphics/geometry/BoxGeometry.h>
#include <math/Vector2.h>
#include <math/Vector3.h>
#include <numeric>
#include <cmath>

BoxGeometry::BoxGeometry(float length, float height, float depth)
{
	float hx = fabs(length) / 2.f;
	float hy = fabs(height) / 2.f;
	float hz = fabs(depth) / 2.f;

	// Four unique vertices per side. 6 sides * 4 vertices = 24
	_vertices = new Vector3[24];
	_numVertices = 24;
	_uvs = new Vector2[24];
	_numUVs = 24;

	// 6 indices per side. 6 sides * 6 indices = 36
	_indices = new unsigned int[36];
	_numIndices = 36;

	unsigned int vIdx = 0;
	unsigned int iIdx = 0;
	createFace(&vIdx, &iIdx, 2, -hz, 0, hx, 1, -hy);		// back
	createFace(&vIdx, &iIdx, 0, -hx, 2, -hz, 1, -hy);		// left
	createFace(&vIdx, &iIdx, 2, hz, 0, -hx, 1, -hy);		// front
	createFace(&vIdx, &iIdx, 0, hx, 2, hz, 1, -hy);			// right
	createFace(&vIdx, &iIdx, 1, hy, 0, -hx, 2, hz);			// top
	createFace(&vIdx, &iIdx, 1, -hy, 0, -hx, 2, -hz);		// bottom
}

BoxGeometryPtr BoxGeometry::create(float length, float height, float depth)
{
	return std::shared_ptr<BoxGeometry>(new BoxGeometry(length, height, depth));
}

void BoxGeometry::createFace(unsigned int* vCount, unsigned int* iCount, int fixedAxis, float fixedValue,
		int axis1, float start1, int axis2, float start2)
{
	Vector3 position;
	float* p[3] = { &position.x, &position.y, &position.z };

	// Point 1
	*p[fixedAxis] = fixedValue;
	*p[axis1] = start1;
	*p[axis2] = start2;
	_uvs[*vCount] = { 0.f, 0.f };
	_indices[(*iCount)++] = *vCount;
	_vertices[(*vCount)++] = position;

	// Point 2
	*p[axis1] *= -1;
	_uvs[*vCount] = { 1.f, 0.f };
	_indices[(*iCount)++] = *vCount;
	_vertices[(*vCount)++] = position;

	// Point 3
	*p[axis2] *= -1;
	_uvs[*vCount] = { 1.f, 1.f };
	_indices[(*iCount)++] = *vCount;
	_indices[(*iCount)++] = *vCount;
	_vertices[(*vCount)++] = position;

	// Point 4
	*p[axis1] *= -1;
	_uvs[*vCount] = { 0.f, 1.f };
	_indices[(*iCount)++] = *vCount;
	_indices[(*iCount)++] = *vCount - 3;
	_vertices[(*vCount)++] = position;
}