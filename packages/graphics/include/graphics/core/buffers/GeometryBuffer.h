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
	 * @param attributes Vertex attributes
	 * @param vertexData Array of vertex data for this buffer
	 * @param numValues The number of values in the vertex data array
	 * @param indices Array containing the vertex indices of this buffer
	 * @param numIndices The number of indices
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
	 * ID of the GLFW vertex buffer object (VBO) that holds the vertex data for this buffer.
	 */
	unsigned int _vboId = 0;

	/**
	 * ID of the GLFW element buffer object (EBO) that holds the index data for this buffer.
	 */
	unsigned int _eboId = 0;

	/**
	 * ID of the GLFW vertex array object (VAO) used to bind and unbind this buffer for rendering.
	 */
	unsigned int _vaoId = 0;
};