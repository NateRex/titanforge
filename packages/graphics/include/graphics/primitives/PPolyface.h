#pragma once
#include <graphics/primitives/IPrimitive.h>
#include <geometry/Polyface.h>

/**
 * A three-dimensional polyface primitive capable of being rendered
 * @author Nathaniel Rex
 */
class PPolyface : public IPrimitive, public Polyface
{
public:
	
	/**
	 * Constructor
	 * @param verts Vertex positions
	 * @param numVerts Number of values in the position array
	 * @param indices Indices pointing to the ordered vertices of this polyface. A value of -1 implies
	 * the end of a facet and the start of another
	 * @param numIndices Number of values in the index array (including -1 values representing the ends
	 * of facets)
	 */
	PPolyface(const Vector3* vertices, int numVerts, const int* indices, int numIndices);

	/**
	 * Constructor
	 * @param vertices Vertex positions
	 * @param indices Indices pointing to the ordered vertices of this polyface. A value of -1 implies
	 * the end of a facet and the start of another
	 */
	PPolyface(const std::vector<Vector3>& vertices, const std::vector<int> indices);

	/**
	 * Constructor
	 * @param polyface Polyface to copy data from
	 */
	PPolyface(const Polyface& polyface);

	/**
	 * Store the data of this primitive into the given buffers
	 * @param vertices Vector to which the unique vertex positions of this primitive should be appended
	 * @param indices Vector to which the vertex indices of this primitive should be appended
	 */
	void buffer(std::vector<float>& vertices, std::vector<int>& indices) const override;

};