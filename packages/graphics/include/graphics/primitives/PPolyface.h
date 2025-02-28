#pragma once
#include <geometry/Polyface.h>

/**
 * A three-dimensional polyface primitive capable of being rendered
 * @author Nathaniel Rex
 */
class PPolyface : public Polyface
{
public:
	
	/**
	 * Constructor
	 * @param pos Vertex positions
	 * @param numPos Number of values in the position array
	 * @param verts Ordered vertices that make up the facets of this polyface. Each value
	 * should represents an index into the list of positions. A value of -1 implies the end
	 * of a facet and the start of another
	 * @param numVerts Number of values in the vertex array (including -1 values representing the ends
	 * of facets)
	 */
	PPolyface(const Vector3* pos, int numPos, const int* verts, int numVerts);

	/**
	 * Constructor
	 * @param positions Vertex positions
	 * @param vertices Ordered vertices that make up the facets of this polyface. Each value
	 * should represents an index into the list of positions. A value of -1 implies the end
	 * of a facet and the start of another
	 */
	PPolyface(const std::vector<Vector3>& positions, const std::vector<int> vertices);

	/**
	 * Constructor
	 * @param polyface Polyface to copy data from
	 */
	PPolyface(const Polyface& polyface);
};