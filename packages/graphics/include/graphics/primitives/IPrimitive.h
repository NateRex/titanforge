#pragma once
#include <vector>

/**
 * Interface for all primitives
 * @author Nathaniel Rex
 */
class IPrimitive
{
public:

	/**
	 * Store the data of this primitive into the given buffers
	 * @param vertices Vector to which the unique vertex positions of this primitive should be appended
	 * @param indices Vector to which the vertex indices of this primitive should be appended
	 */
	virtual void buffer(std::vector<float>& vertices, std::vector<int>& indices) const = 0;
};