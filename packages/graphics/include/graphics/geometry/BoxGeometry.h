#pragma once
#include <graphics/geometry/pointers/GeometryPtr.h>
#include <graphics/geometry/pointers/BoxGeometryPtr.h>
#include <graphics/geometry/Geometry.h>

/**
 * Geometry representing a three-dimensional rectangular cuboid. On creation, the box is centered
 * at the origin in local space.
 * @author Nathaniel Rex
 */
class BoxGeometry : public Geometry
{
public:

	/**
	 * Constructs a new box geometry instance
	 * @param length Length of the box along the x axis
	 * @param height Height of the box along the y axis
	 * @param depth Depth of the box along the z axis
	 * @return The new box geometry instance
	 */
	static BoxGeometryPtr create(float length, float height, float depth);

private:
	
	/**
	 * Constructor
	 * @param length Length of the box along the x axis
	 * @param height Height of the box along the y axis
	 * @param depth Depth of the box along the z axis
	 */
	BoxGeometry(float length, float height, float depth);

	/**
	 * Helper method to build one of the faces of a box geometry. This method assumes that the buffers of this
	 * geometry have already been allocated with enough memory to store the results.
	 * @param vCount The total number of unique vertices that have been created so far, across all facets. This value
	 * will be updated by this method upon the creation of new vertices.
	 * @param iCount The total number of indices that have been registered so far, across all facets. This value will
	 * be updated by this method upon the creation of new indices.
	 * @param fixedAxis Fixed axis of this facet (0 for x, 1 for y, and 2 for z)
	 * @param fixedValue Fixed axis value
	 * @param axis1 First variable axis (0 for x, 1 for y, and 2 for z)
	 * @param start1 Starting value for the first of the two variable axes
	 * @param axis2 Second variable axis (0 for x, 1 for y, and 2 for z)
	 * @param start2 Starting value for the second of the two variable axes
	 */
	void createFace(unsigned int* vCount, unsigned int* iCount, int fixedAxis, float fixedValue,
		int axis1, float start1, int axis2, float start2);
};