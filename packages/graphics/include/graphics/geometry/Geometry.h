#pragma once
#include <graphics/geometry/GeometryPtr.h>

class Vector2;
class Vector3;
class Color;

/**
 * Base class for all geometry, which defines an object, line, or point in local space. Contains vertex attributes
 * such as position, color, texture coordinates, and unit normals.
 * @author Nathaniel Rex
 */
class Geometry
{
public:

	/**
	 * Destructor
	 */
	~Geometry();

	/**
	 * Constructs a new geometry instance
	 */
	static GeometryPtr create();

	/**
	 * Sets the vertex positions of this geometry.
	 * @param vertices An array where every 3 values represents the x, y, and z components of a vertex.
	 * @param numVertices The number of vertices in the array
	 */
	void setVertices(const float* vertices, unsigned int numVertices);

	/**
	 * Sets the vertex indices of this geometry.
	 * @param indices An array whose values point to vertices, defining the ordering of positions for this geometry.
	 * @param numIndices The number of indices in the array
	 */
	void setIndices(const unsigned int* indices, unsigned int numIndices);

	/**
	 * @return The total number of indexed vertices in this geometry
	 */
	unsigned int size() const;

	/**
	 * Sets the color for each vertex in this geometry.
	 * @param colors An array where every 4 values represent the r, g, b, and a components of a color. Each value
	 * should be in the range 0 to 1.
	 * @param numColors The number of colors in the array. This is expected to match the number of
	 * vertices in this geometry.
	 */
	void setColors(const float* colors, unsigned int numColors);

	/**
	 * Removes vertex colors from this geometry, if currently present.
	 */
	void removeColors();

	/**
	 * @return True if this geometry contains per-vertex colors. Returns false otherwise.
	 */
	bool hasColors() const;

	/**
	 * Sets the texture coordinate for each vertex in this geometry.
	 * @param uvs An array where every 2 values represents the u and v components of a texture coordinate. Each
	 * value should be in the range 0 to 1.
	 * @param numUVs The number of texture coordinates in the array. This is expected to match the number of
	 * vertices in this geometry.
	 */
	void setTextureCoords(const float* uvs, unsigned int numUVs);

	/**
	 * Removes vertex texture coordinates from this geometry, if currently present.
	 */
	void removeTextureCoords();

	/**
	 * @return True if this geometry contains per-vertex texture coordinates. Returns false otherwise.
	 */
	bool hasTextureCoords() const;

protected:

	/**
	 * Vertex positions. These positions may be referenced by one or more indices.
	 */
	Vector3* _vertices = nullptr;

	/**
	 * Number of vertex positions.
	 */
	unsigned int _numVertices = 0;

	/**
	 * Ordered vertex indices. These values point to positions in the vertices list.
	 */
	unsigned int* _indices = nullptr;

	/**
	 * Number of vertex indices.
	 */
	unsigned int _numIndices = 0;

	/**
	 * Vertex colors. Can be null, implying no per-vertex coloring. When not empty,
	 * the size of this list should be equal to the number of vertices.
	 */
	Color* _colors = nullptr;

	/**
	 * Number of colors
	 */
	unsigned int _numColors = 0;

	/**
	 * Texture coordinates. Can be null, implying no per-vertex texture mapping. When not
	 * empty, the size of this list should be equal to the number of vertices.
	 */
	Vector2* _uvs = nullptr;

	/**
	 * Number of texture coordinates.
	 */
	unsigned int _numUVs = 0;

	/**
	 * Constructor
	 */
	Geometry();
};