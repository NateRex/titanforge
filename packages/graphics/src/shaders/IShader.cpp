#include <graphics/shaders/IShader.h>

IShader::IShader(const std::string& name, const std::string& source)
	: _name(name), _src(source)
{

}

const std::string IShader::getName() const
{
	return _name;
}

const std::string IShader::getSrc() const
{
	return _src;
}

void IShader::operator=(const IShader& shader)
{
	_name = shader._name;
	_src = shader._src;
}