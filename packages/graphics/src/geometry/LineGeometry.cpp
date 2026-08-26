#include <graphics/geometry/LineGeometry.h>
#include <math/Vector3.h>
#include <algorithm>

LineGeometry::LineGeometry(const Vector3* points, unsigned int numPoints, PrimitiveType primitiveType): Geometry(primitiveType)
{
    _numVertices = numPoints;
    
    if (numPoints < 2) return;

    _vertices = new Vector3[numPoints];
    std::copy_n(points, numPoints, _vertices);
}

LineGeometryPtr LineGeometry::createSegments(const Vector3* points, unsigned int numPoints)
{
    return std::shared_ptr<LineGeometry>(new LineGeometry(points, numPoints, PrimitiveType::LINE_SEGMENTS));
}

LineGeometryPtr LineGeometry::createPolyline(const Vector3* points, unsigned int numPoints, bool closed)
{
    return std::shared_ptr<LineGeometry>(new LineGeometry(points, numPoints,
            closed ? PrimitiveType::CLOSED_POLYLINE : PrimitiveType::POLYLINE));
}