#pragma once
#include <stdexcept>

/**
 * Exception used in cases where an element in a data structure was requested
 * using an index that was not within the valid range.
 * @author Nathaniel Rex
 */
class OutOfBoundsException : public std::runtime_error
{
public:

	/**
	 * Constructor
	 * @param msg Exception message
	 */
	OutOfBoundsException(const std::string& msg);

	/**
	 * @override
	 */
	const char* what() const noexcept override;
};