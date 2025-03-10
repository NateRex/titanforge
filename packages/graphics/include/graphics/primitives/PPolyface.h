#pragma once
#include <graphics/primitives/IPrimitive.h>
#include <geometry/Polyface.h>

class Color;

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
	 * @param colors (Optional) Array of colors for each index. When present, the number of values is
	 * expected to match the number of indices.
	 */
	PPolyface(const Vector3* vertices, int numVerts, const int* indices, int numIndices,
			const Color* colors = nullptr);

	/**
	 * Constructor
	 * @param vertices Vertex positions
	 * @param indices Indices pointing to the ordered vertices of this polyface. A value of -1 implies
	 * the end of a facet and the start of another
	 * @param colors (Optional) Colors for each index. When present, the number of colors is expected
	 * to match the number of indices.
	 */
	PPolyface(const std::vector<Vector3>& vertices, const std::vector<int> indices,
			const std::vector<Color> colors = std::vector<Color>());

	/**
	 * Constructor
	 * @param polyface Polyface to copy data from
	 */
	PPolyface(const Polyface& polyface);

	/**
	 * @return The set of attributes defining what data is contained by this primitive
	 */
	PrimitiveAttributes getAttributes() const override;

	/**
	 * Store the data of this primitive into the given buffers
	 * @param vertexData Vector in which the data for each unique vertex should be stored.
	 * @param indices Vector to which the vertex indices of this primitive should be appended
	 */
	void buffer(std::vector<float>& vertexData, std::vector<int>& indices) const override;

protected:

	/**
	 * Vertex colors
	 */
	std::vector<Color> _colors;
};