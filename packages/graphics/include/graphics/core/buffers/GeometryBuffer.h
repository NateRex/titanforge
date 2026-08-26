#pragma once

class GeometryAttributes;

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
	 * OpenGL vertex buffer object (VBO) name.
	 */
	unsigned int _vboId = 0;

	/**
	 * OpenGL element buffer object (EBO) name. May remain undefined if the geometry does not define indices.
	 */
	unsigned int _eboId = 0;

	/**
	 * OpenGL vertex array object (VAO) name used to bind this buffer for rendering.
	 */
	unsigned int _vaoId = 0;

	/**
	 * The total number of points in this buffer. If indices are set, this will be the number of indices. Otherwise, it defaults
	 * to the number of vertices.
	 */
	unsigned int _size;
};
