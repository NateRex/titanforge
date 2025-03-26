#pragma once
#include <vector>

class Vector2;
class Vector3;
class Color;

/**
 * Base class for all geometry, used to represent a mesh, line, or point. Contains vertex attributes
 * such as position, color, texture coordinates, and unit normals.
 * @author Nathaniel Rex
 */
class Geometry
{
public:

	/**
	 * Constructor
	 */
	Geometry();

	/**
	 * @return The total number of indexed vertices in this geometry
	 */
	unsigned int size() const;

	/**
	 * Sets the vertex positions of this geometry.
	 * @param vertices An array where every 3 values represents the x, y, and z components of a vertex
	 * @param numVertices The number of vertices in the array
	 */
	void setVertices(const float* vertices, unsigned int numVertices);

	/**
	 * Sets the vertex indices of this geometry.
	 * @param indices An array whose values point to vertices, defining the ordering of positions for
	 * this geometry.
	 * @param numIndices The number of indices in the array
	 */
	void setIndices(const unsigned int* indices, unsigned int numIndices);

	/**
	 * Sets the color for each vertex in this geometry.
	 * @param colors An array where every 4 values represent the r, g, b, and a components of a color. 
	 * @param numColors The number of colors in the array. This is expected to match the number of
	 * vertices in this geometry.
	 */
	void setColors(const float* colors, unsigned int numColors);

	/**
	 * Sets the texture coordinate for each vertex in this geometry.
	 * @param uvs An array where every 2 values represents the u and v components of a texture coordinate.
	 * @param numUVs The number of texture coordinates in the array. This is expected to match the number of
	 * vertices in this geometry.
	 */
	void setTextureCoords(const float* uvs, unsigned int numUVs);

private:

	/**
	 * Vertex positions. These positions may be referenced by one or more indices.
	 */
	std::vector<Vector3> _vertices;

	/**
	 * Ordered vertex indices. These values point to positions in the vertices list.
	 */
	std::vector<unsigned int> _indices;

	/**
	 * Vertex colors. Can be empty, implying no per-vertex coloring. When not empty,
	 * the size of this list should be equal to the number of vertices.
	 */
	std::vector<Color> _colors;

	/**
	 * Texture coordinates. Can be empty, implying no per-vertex texture mapping. When not
	 * empty, the size of this list should be equal to the number of vertices.
	 */
	std::vector<Vector2> _uvs;
};