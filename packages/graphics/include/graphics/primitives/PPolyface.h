#pragma once
#include <graphics/primitives/IPrimitive.h>
#include <geometry/Polyface.h>

class Color;
class Vector2;

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
	 * @param colors (Optional) Array of colors for each vertex position. When present, the number of values is
	 * expected to match the number of unique vertices.
	 * @param texCoords (Optional) Array of texture coordinates for each index. When present, the number of
	 * values is expected to match the number of unique vertices.
	 */
	PPolyface(const Vector3* vertices, int numVerts, const int* indices, int numIndices,
			const Color* colors = nullptr, const Vector2* texCoords = nullptr);

	/**
	 * Constructor
	 * @param polyface Polyface to copy data from
	 * @param colors (Optional) Array of colors for each vertex position. When present, the number of values
	 * is expected to match the number of unique vertices in the polyface.
	 * @param texCoords (Optional) Array of texture coordinates for each vertex position. When present, the number of
	 * values is expected to match the number of unique vertices in the polyface.
	 */
	PPolyface(const Polyface& polyface, const Color* colors = nullptr, const Vector2* texCoords = nullptr);

	/**
	 * Destructor
	 */
	~PPolyface();

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
	 * Colors for each vertex index. Can be empty, in which case it is assumed this polyface does not contain
	 * color.
	 */
	std::vector<Color> _colors;

	/**
	 * Texture coordinates for each vertex index. Can be empty, in which case it is assumed this polyface does
	 * not contain texture.
	 */
	std::vector<Vector2> _texCoords;
};