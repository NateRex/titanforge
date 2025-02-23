#include <common/exceptions/NullPointerException.h>

NullPointerException::NullPointerException(const std::string& msg) : std::runtime_error(msg)
{

}

const char* NullPointerException::what() const noexcept
{
	return std::runtime_error::what();
}
