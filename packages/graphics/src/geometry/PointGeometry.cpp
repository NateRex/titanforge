#include <graphics/geometry/PointGeometry.h>
#include <algorithm>

PointGeometry::PointGeometry(const Vector3* points, unsigned int numPoints)
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

PointGeometryPtr PointGeometry::create(const std::vector<Vector3>& points)
{
    return std::shared_ptr<PointGeometry>(new PointGeometry(points.data(), points.size()));
}

PointGeometryPtr PointGeometry::create(std::initializer_list<Vector3> points)
{
    return std::shared_ptr<PointGeometry>(new PointGeometry(points.begin(), points.size()));
}