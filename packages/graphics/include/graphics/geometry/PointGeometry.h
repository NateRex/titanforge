#pragma once
#include <graphics/geometry/pointers/PointGeometryPtr.h>
#include <graphics/geometry/Geometry.h>

class Vector3;

/**
 * Geometry used to represent one or more point primitives. The point positions are assumed to be defined in world space.
 * @author Nathaniel Rex
 */
class PointGeometry : public Geometry
{
public:

    /**
     * Constructs a new point geometry instance containing a single point
     * @param point A single point defined in world-space coordinates
     * @return The new point geometry instance
     */
    static PointGeometryPtr create(const Vector3& point);

    /**
     * Constructs a new point geometry instance
     * @param points The set of points defined in world-space coordinates
     * @param numPoints The number of points
     * @return The new point geometry instance
     */
    static PointGeometryPtr create(const Vector3* points, unsigned int numPoints);

private:

    /**
     * Constructor
     * @param points The set of points defined in world-space coordinates
     * @param numPoints The number of points
     */
    PointGeometry(const Vector3* points, unsigned int numPoints);
};