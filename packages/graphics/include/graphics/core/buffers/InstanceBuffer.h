#pragma once
#include <graphics/core/buffers/GeometryBuffer.h>
#include <vector>

class Matrix4;

/**
 * An extension of GeometryBuffer capable of containing geometry storage as well as per-instance transformations. This
 * is buffer is specifically used for instancing.
 * @author Nathaniel Rex
 */
class InstanceBuffer : public GeometryBuffer
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
	InstanceBuffer(const GeometryAttributes& attributes, const float* vertexData, unsigned int numValues,
		const unsigned int* indices, unsigned int numIndices);

	/**
	 * Destructor
	 */
	~InstanceBuffer();

	/**
	 * Replaces the instance transforms
	 * @param transforms The transforms for each instance
	 */
	void setTransforms(const std::vector<Matrix4>& transforms);

private:

	/**
	 * ID of the OpenGL vertex buffer object (VBO) used to store instance transform data
	 */
	unsigned int _instanceVBO = 0;

};
