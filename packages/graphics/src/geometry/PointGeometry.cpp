#include <graphics/geometry/PointGeometry.h>
#include <math/Vector3.h>
#include <algorithm>

PointGeometry::PointGeometry(const Vector3* points, unsigned int numPoints): Geometry(PrimitiveType::POINTS)
{
    _numVertices = numPoints;
    
    if (numPoints == 0) return;

    _vertices = new Vector3[numPoints];
    std::copy_n(points, numPoints, _vertices);
}

PointGeometryPtr PointGeometry::create(const Vector3& point)
{
    return std::shared_ptr<PointGeometry>(new PointGeometry(&point, 1));
}

PointGeometryPtr PointGeometry::create(const Vector3* points, unsigned int numPoints)
{
    return std::shared_ptr<PointGeometry>(new PointGeometry(points, numPoints));
}