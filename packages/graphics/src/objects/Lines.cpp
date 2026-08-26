#include <graphics/objects/Lines.h>
#include <graphics/geometry/LineGeometry.h>

LinesPtr Lines::createSegments(const std::vector<Vector3>& points, MaterialPtr material)
{
    LineGeometryPtr geom = LineGeometry::createSegments(points.data(), points.size());
    return std::shared_ptr<Lines>(new Lines(geom, material));
}

LinesPtr Lines::createSegments(std::initializer_list<Vector3> points, MaterialPtr material)
{
    LineGeometryPtr geom = LineGeometry::createSegments(points.begin(), points.size());
    return std::shared_ptr<Lines>(new Lines(geom, material));
}

LinesPtr Lines::createPolyline(const std::vector<Vector3>& points, MaterialPtr material, bool closed)
{
    LineGeometryPtr geom = LineGeometry::createPolyline(points.data(), points.size(), closed);
    return std::shared_ptr<Lines>(new Lines(geom, material));
}

LinesPtr Lines::createPolyline(std::initializer_list<Vector3> points, MaterialPtr material, bool closed)
{
    LineGeometryPtr geom = LineGeometry::createPolyline(points.begin(), points.size(), closed);
    return std::shared_ptr<Lines>(new Lines(geom, material));
}