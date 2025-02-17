#include <common/exceptions/OutOfBoundsException.h>

OutOfBoundsException::OutOfBoundsException(const std::string& msg) : std::runtime_error(msg)
{

}

const char* OutOfBoundsException::what() const noexcept
{
	return std::runtime_error::what();
}
