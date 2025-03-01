#pragma once
#include <stdexcept>

/**
 * Exception used in cases where a method was invoked at an illegal or inappropriate time
 * object method.
 * @author Nathaniel Rex
 */
class IllegalStateException : public std::runtime_error
{
public:

	/**
	 * Constructor
	 * @param msg Exception message
	 */
	IllegalStateException(const std::string& msg);

	/**
	 * @override
	 */
	const char* what() const noexcept override;
};