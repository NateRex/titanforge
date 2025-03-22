#pragma once
#include <vector>

class PrimitiveAttributes;

/**
 * Interface for all primitives
 * @author Nathaniel Rex
 */
class IPrimitive
{
public:

	/**
	 * @return The set of attributes defining what data is contained by this primitive
	 */
	virtual PrimitiveAttributes getAttributes() const = 0;

	/**
	 * Transforms the data of this primitive into world space, and stores it in the given buffers
	 * @param vertexData Vector in which the data for each unique vertex should be stored.
	 * @param indices Vector to which the vertex indices of this primitive should be appended
	 */
	virtual void buffer(std::vector<float>& vertexData, std::vector<int>& indices) const = 0;
};