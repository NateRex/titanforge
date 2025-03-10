#pragma once
#include <string>

class PrimitiveAttributes;

/**
 * A buffer capable of being loaded with vertex array data to be sent to the GPU
 * @author Nathaniel Rex
 */
class Buffer
{
public:

	friend class BufferBuilder;
	friend class BufferManager;

	/**
	 * Unique name of this buffer
	 */
	const std::string name;

private:

	/**
	 * ID of the GLFW vertex buffer object (VBO) that holds the vertex data for this buffer. Will be
	 * zero until the OpenGL resources for this buffer are explicitly constructed via create().
	 */
	unsigned int _vboId;

	/**
	 * ID of the GLFW element buffer object (EBO) that holds the index data for this buffer. Will be
	 * zero until the OpenGL resources for this buffer are explicitly constructed via create().
	 */
	unsigned int _eboId;

	/**
	 * ID of the GLFW vertex array object (VAO) used to bind and unbind this buffer for rendering. Will
	 * be zero until the OpenGL resources for this buffer are explicitly constructed via create().
	 */
	unsigned int _vaoId;

	/**
	 * The number of indices held inside this buffer. Will be zero until the OpenGL resources for this
	 * buffer are explicitly constructed via create().
	 */
	unsigned int _size;

	/**
	 * Constructor
	 * @param name The unique name of the buffer
	 */
	Buffer(const std::string& name);

	/**
	 * Computes the stride of a buffer given the attributes of a primitive
	 * @param attributes The primitive attributes
	 * @return The buffer stride
	 */
	static unsigned int computeStride(const PrimitiveAttributes& attributes);

	/**
	 * Creates the OpenGL resources for this buffer, loading them with the given data.
	 * @param attributes Primitive attributes
	 * @param vertexData Array of vertex data for this buffer
	 * @param numValues The number of values in the vertex data array
	 * @param indices Array containing the vertex indices of this buffer
	 * @param numIndices The number of indices
	 */
	void create(const PrimitiveAttributes& attributes, const float* vertexData, unsigned int numValues,
			const int* indices, unsigned int numIndices);

	/**
	 * Destroys this buffer, freeing its resources
	 */
	void destroy();
};