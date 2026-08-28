#pragma once
#include <graphics/geometry/GeometryAttributes.h>

/**
 * A buffer responsible for holding the vertex array data of a geometry
 * @author Nathaniel Rex
 */
class GeometryBuffer
{
public:

	/**
	 * Constructor
	 * @param attributes Geometry attributes
	 * @param vertexData Array of vertex data for this buffer
	 * @param numValues The number of values in the vertex data array
	 * @param indices Array containing the vertex indices of this buffer. Can be null if the geometry does not define indices.
	 * @param numIndices The number of indices, if present
	 */
	GeometryBuffer(const GeometryAttributes& attributes, const float* vertexData, unsigned int numValues,
		const unsigned int* indices, unsigned int numIndices);

	/**
	 * Destructor
	 */
	~GeometryBuffer();

	/**
	 * @return The total number of points in this buffer. If indices are set, this will be the number of indices. Otherwise, it defaults
	 * to the number of vertices.
	 */
	unsigned int size() const { return _size; }

	/**
	 * Binds this buffer for drawing
	 */
	void bind() const;

private:

	/**
	 * ID of the OpenGL vertex buffer object (VBO) used to store geometry vertex data
	 */
	unsigned int _geometryVBO = 0;

	/**
	 * ID of the OpenGL element buffer object (EBO) used to store geometry index data. May be 0 if the geometry does not define indices.
	 */
	unsigned int _geometryEBO = 0;

	/**
	 * ID of the OpenGL vertex array object (VAO)
	 */
	unsigned int _vao = 0;

	/**
	 * Layout of each vertex in the VBO.
	 */
	GeometryAttributes _attributes;

	/**
	 * The total number of points in this buffer. If indices are set, this will be the number of indices. Otherwise, it defaults
	 * to the number of vertices.
	 */
	unsigned int _size;
};
