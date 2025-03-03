#include <graphics/shaders/ShaderProgram.h>
#include <common/Assertions.h>

ShaderProgram::ShaderProgram(const std::string& name, const std::initializer_list<std::string>& shaders)
	: _id(0), _name(name), _shaders(shaders)
{
	assertGreaterThan(_shaders.size(), 0, "Shader program must reference atleast one shader");
}

ShaderProgram::ShaderProgram(const std::string& name, const std::vector<std::string>& shaders)
	: _id(0), _name(name), _shaders(shaders)
{

}

const std::string ShaderProgram::getName() const
{
	return _name;
}

void ShaderProgram::operator=(const ShaderProgram& program)
{
	_id = program._id;
	_name = program._name;
	_shaders = program._shaders;
}