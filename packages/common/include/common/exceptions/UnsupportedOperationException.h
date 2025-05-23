#pragma once
#include <stdexcept>

/**
 * Exception used in cases where a requested operation is not supported by an object
 * @author Nathaniel Rex
 */
class UnsupportedOperationException : public std::runtime_error
{
public:

	/**
	 * Constructor
	 * @param msg Exception message
	 */
	UnsupportedOperationException(const std::string& msg);

	/**
	 * @override
	 */
	const char* what() const noexcept override;
};