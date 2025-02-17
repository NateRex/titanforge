#pragma once
#include <stdexcept>

/**
 * Exception used in cases where a caller provided invalid input to a function or
 * object method.
 * @author Nathaniel Rex
 */
class IllegalArgumentException : public std::runtime_error
{
public:

	/**
	 * Constructor
	 * @param msg Exception message
	 */
	IllegalArgumentException(const std::string& msg);

	/**
	 * @override
	 */
	const char* what() const noexcept override;
};