#pragma once
#include <stdexcept>

/**
 * Exception used in cases where the program attempts to reference an object having a null value
 * @author Nathaniel Rex
 */
class NullPointerException : public std::runtime_error
{
public:

	/**
	 * Constructor
	 * @param msg Exception message
	 */
	NullPointerException(const std::string& msg);

	/**
	 * @override
	 */
	const char* what() const noexcept override;
};