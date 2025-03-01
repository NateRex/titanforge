#pragma once

class Buffer;

/**
 * Interface for all primitives
 * @author Nathaniel Rex
 */
class IPrimitive
{
public:

	/**
	 * Store the data of this primitive into the given buffer
	 * @param buffer Buffer object
	 */
	virtual void buffer(Buffer& buffer) const = 0;
};