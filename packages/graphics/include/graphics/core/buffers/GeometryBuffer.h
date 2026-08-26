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
	 * The number of indices held inside this buffer
	 */
	const unsigned int size;

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
};
