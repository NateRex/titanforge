#include <common/exceptions/IllegalArgumentException.h>

IllegalArgumentException::IllegalArgumentException(const std::string& msg) : std::runtime_error(msg)
{

}

const char* IllegalArgumentException::what() const noexcept
{
	return std::runtime_error::what();
}
