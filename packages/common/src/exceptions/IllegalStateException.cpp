#include <common/exceptions/IllegalStateException.h>

IllegalStateException::IllegalStateException(const std::string& msg) : std::runtime_error(msg)
{

}

const char* IllegalStateException::what() const noexcept
{
	return std::runtime_error::what();
}
