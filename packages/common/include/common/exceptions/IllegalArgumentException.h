#pragma once
#include <stdexcept>

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