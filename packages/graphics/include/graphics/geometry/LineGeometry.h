#pragma once
#include <graphics/geometry/pointers/LineGeometryPtr.h>
#include <graphics/geometry/Geometry.h>

/**
 * Geometry used to represent a sequence of points forming line segments.
 * 
 * These points may either be interpreted as disjoint pairs (where each consecutive pair defines an independent line segment),
 * or as a connected polyline (where consecutive points define adjoining segments).
 * 
 * A connected polylines may optionally be closed by connecting its final point to its first point.
 * 
 * @author Nathaniel Rex
 */
class LineGeometry : public Geometry
{
public:

    /**
     * Constructs a line geometry consisting of disjoint segments
     * @param points The points that make up the line segments. Each pair of points defines a single segment. If an odd number
     * of points is provided, the last point will be ignored.
     * @param numPoints The number of points
     * @return The newly-constructed line geometry
     */
    static LineGeometryPtr createSegments(const Vector3* points, unsigned int numPoints);

    /**
     * Constructs a polyline, where consecutive points define adjoining segments.
     * @param points The points that make up the polyline
     * @param numPoints The number of points
     * @param closed Boolean flag that, when true, automatically closes the polyline by connecting the final point to the first
     * point from the given array. Defaults to false.
     * @return The newly-constructed line geometry
     */
    static LineGeometryPtr createPolyline(const Vector3* points, unsigned int numPoints, bool close = false);

private:

    /**
     * Constructor
     * @param points The set of points
     * @param numPoints The number of points
     * @param primitiveType The geometric primitive type used to interpret the vertex data of this geometry
     */
    LineGeometry(const Vector3* points, unsigned int numPoints, PrimitiveType primitiveType);
};