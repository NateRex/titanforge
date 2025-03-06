#pragma once
#include <string>

/**
 * A buffer capable of being loaded with vertex array data to be sent to the GPU
 * @author Nathaniel Rex
 */
class Buffer
{
public:

	friend class BufferBuilder;
	friend class BufferManager;

private:

	/**
	 * GLFW vertex buffer object (VBO) ID. Will be zero until the OpenGL resources for this buffer
	 * are explicitly constructed via create().
	 */
	unsigned int _vboId;

	/**
	 * GLFW vertex array object (VAO) ID. Will be zero until the OpenGL resources for this buffer
	 * are explicitly constructed via create().
	 */
	unsigned int _vaoId;

	/**
	 * Unique name of this buffer
	 */
	std::string _name;

	/**
	 * Constructor
	 * @param name The unique name of the buffer
	 */
	Buffer(const std::string& name);

	/**
	 * Creates the OpenGL resources for this buffer, loading them with the given data.
	 * @param data The data array
	 * @param size The number of values stored in the array
	 */
	void create(const float* data, unsigned int size);

	/**
	 * Destroys this buffer, freeing its resources
	 */
	void destroy();
};