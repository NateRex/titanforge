#pragma once
#include <graphics/objects/pointers/LinesPtr.h>
#include <graphics/geometry/LineGeometry.h>
#include <graphics/objects/Renderable.h>
#include <vector>
#include <initializer_list>

class Vector3;

/**
 * Represents a collection of points forming line segments.
 * 
 * These points may either be interpreted as disjoint line segments or as a connected polyline.
 * 
 * A connected polylines may optionally be closed by connecting its final point to its first point.
 * 
 * @author Nathaniel Rex
 */
class Lines : public Renderable
{
public:

    /**
     * Constructs an object consisting of disjoint line segments
     * @param points The points that make up the line segments. Each pair of points defines a single segment. If an odd number
     * of points is provided, the last point will be ignored
     * @param material Line material
     * @return The new lines instance
     */
    static LinesPtr createSegments(const std::vector<Vector3>& points, MaterialPtr material);

    /**
     * Constructs an object consisting of disjoint line segments
     * @param points The points that make up the line segments. Each pair of points defines a single segment. If an odd number
     * of points is provided, the last point will be ignored
     * @param material Line material
     * @return The new lines instance
     */
    static LinesPtr createSegments(std::initializer_list<Vector3> points, MaterialPtr material);

    /**
     * Constructs an object consisting of points connected to form a single polyline
     * @param points The points that make up the polyline
     * @param material Line material
     * @param closed Boolean flag that, when true, automatically closes the polyline by connecting the final point to the first
     * point from the given array. Defaults to false.
     * @return The new lines instance
     */
    static LinesPtr createPolyline(const std::vector<Vector3>& points, MaterialPtr material, bool closed = false);

    /**
     * Constructs an object consisting of points connected to form a single polyline
     * @param points The points that make up the polyline
     * @param material Line material
     * @param closed Boolean flag that, when true, automatically closes the polyline by connecting the final point to the first
     * point from the given array. Defaults to false.
     * @return The new lines instance
     */
    static LinesPtr createPolyline(std::initializer_list<Vector3> points, MaterialPtr material, bool closed = false);

    /**
     * Constructs a new lines object
     * @param geometry Line geometry
     * @param material Material
     * @return The new lines instance
     */
    static LinesPtr create(LineGeometryPtr geometry, MaterialPtr material)
    {
        return std::shared_ptr<Lines>(new Lines(geometry, material));
    }

protected:

    /**
     * Constructor
     * @param geometry Line geometry
     * @param material Material
     */
    Lines(LineGeometryPtr geometry, MaterialPtr material) : Renderable(geometry, material) {}
};