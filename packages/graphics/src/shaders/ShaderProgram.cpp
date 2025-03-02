#include <graphics/shaders/ShaderProgram.h>
#include <common/Assertions.h>

ShaderProgram::ShaderProgram(const std::string& name, const std::initializer_list<std::string>& shaders)
	: _id(0), _name(name), _shaders(shaders)
{
	assertGreaterThan(_shaders.size(), 0, "Shader program must reference atleast one shader");
}

const std::string ShaderProgram::getName() const
{
	return _name;
}