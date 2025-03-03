#pragma once
#include <string>
#include <vector>

/**
 * A program composed of one or more shaders, managed by the shader manager
 * @author Nathaniel Rex
 */
class ShaderProgram
{
public:

	friend class ShaderManager;

	/**
	 * Constructor
	 * @param name The name of this shader program
	 * @param shaders List consisting of the names of shaders that should be linked
	 * as part of this program. Each shader will be resolved using those mounted to the shader
	 * manager.
	 */
	ShaderProgram(const std::string& name, const std::vector<std::string>& shaders);

	/**
	 * Constructor
	 * @param name The name of this shader program
	 * @param shaders Initializer list consisting of the names of shaders that should be linked
	 * as part of this program. Each shader will be resolved using those mounted to the shader
	 * manager.
	 */
	ShaderProgram(const std::string& name, const std::initializer_list<std::string>& shaders);

	/**
	 * @return The unique name of this shader program
	 */
	const std::string getName() const;

private:

	/**
	 * GLFW id. Will be zero until this program has been linked.
	 */
	unsigned int _id;

	/**
	 * Unique name of this shader program
	 */
	std::string _name;

	/**
	 * The names of the shaders linked as part of this shader program
	 */
	std::vector<std::string> _shaders;

	/**
	 * Assignment operator for copying data from another shader program
	 * @param program Shader program to copy data from
	 */
	void operator=(const ShaderProgram& shader);
};