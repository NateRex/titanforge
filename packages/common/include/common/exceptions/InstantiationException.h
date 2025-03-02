#pragma once
#include <stdexcept>

/**
 * Exception used in cases when an object could not be instantiated
 * @author Nathaniel Rex
 */
class InstantiationException : public std::runtime_error
{
public:

	/**
	 * Constructor
	 * @param msg Exception message
	 */
	InstantiationException(const std::string& msg);

	/**
	 * @override
	 */
	const char* what() const noexcept override;
};