#pragma once

/**
 * Representation of an OpenGL buffer used to store data that will be sent to the GPU
 * @author Nathaniel Rex
 */
class Buffer
{
public:

	friend class Engine;

	/**
	 * Add vertex data to this buffer.
	 * @param vertices Array of vertices.
	 * @param numVertices The number of vertices in the array.
	 */
	void addVertices(const float* vertices, int numVertices);

private:

	/**
	 * GLFW buffer ID
	 */
	unsigned int _id;

	/**
	 * Constructor. Requires the graphics engine to have been initialized.
	 */
	Buffer();
};