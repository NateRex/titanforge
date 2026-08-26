#include <graphics/objects/Points.h>
#include <graphics/geometry/PointGeometry.h>

PointsPtr Points::create(const Vector3& point, MaterialPtr material)
{
    PointGeometryPtr geometry = PointGeometry::create(point);
    return std::shared_ptr<Points>(new Points(geometry, material));
}

PointsPtr Points::create(const std::vector<Vector3>& points, MaterialPtr material)
{
    PointGeometryPtr geometry = PointGeometry::create(points.data(), points.size());
    return std::shared_ptr<Points>(new Points(geometry, material));
}

PointsPtr Points::create(std::initializer_list<Vector3> points, MaterialPtr material)
{
    PointGeometryPtr geometry = PointGeometry::create(points.begin(), points.size());
    return std::shared_ptr<Points>(new Points(geometry, material));
}