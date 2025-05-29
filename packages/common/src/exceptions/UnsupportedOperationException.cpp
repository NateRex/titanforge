#include <common/exceptions/UnsupportedOperationException.h>

UnsupportedOperationException::UnsupportedOperationException(const std::string& msg) : std::runtime_error(msg)
{

}

const char* UnsupportedOperationException::what() const noexcept
{
	return std::runtime_error::what();
}
