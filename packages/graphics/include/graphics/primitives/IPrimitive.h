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
	 * Store the data of this primitive into the given buffer
	 * @param buffer Vector containing primitive data
	 */
	virtual void buffer(std::vector<float>& buffer) const = 0;
};