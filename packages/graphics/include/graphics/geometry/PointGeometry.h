#pragma once
#include <graphics/geometry/pointers/PointGeometryPtr.h>
#include <graphics/geometry/Geometry.h>
#include <math/Vector3.h>
#include <vector>

/**
 * Geometry used to represent one or more point primitives. The point positions are assumed to be defined in world space.
 * @author Nathaniel Rex
 */
class PointGeometry : public Geometry
{
public:

    /**
	 * Constructs a new point geometry instance
	 * @param points The set of points defined using world-space coordinates
	 * @return The new point geometry instance
	 */
	static PointGeometryPtr create(const std::vector<Vector3>& points);

    /**
     * Constructs a new point geometry instance
     * @param points The set of points defined using world-space coordinates
     * @return The new point geometry instance
     */
    static PointGeometryPtr craete(std::initializer_list<Vector3> points);

    
};