#pragma once

/**
 * Representation of an OpenGL buffer used to store data that will be sent to the GPU
 * @author Nathaniel Rex
 */
class Buffer
{
public:

	/**
	 * Buffer ID
	 */
	const unsigned int id;

	/**
	 * Constructor. Requires the graphics engine to have been initialized.
	 */
	Buffer();

	/**
	 * Add vertex data to this buffer.
	 * @param vertices Array of vertices.
	 * @param numVertices The number of vertices in the array.
	 */
	addVertices(const double* vertices, )
};