#include <common/exceptions/InstantiationException.h>

InstantiationException::InstantiationException(const std::string& msg) : std::runtime_error(msg)
{

}

const char* InstantiationException::what() const noexcept
{
	return std::runtime_error::what();
}
