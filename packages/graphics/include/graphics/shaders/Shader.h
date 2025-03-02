#pragma once
#include <string>

/**
 * A shader capable of being compiled and linked into a shader program
 * @author Nathaniel Rex
 */
class Shader
{
public:

	friend class ShaderManager;

	/**
	 * Constructor
	 * @param type Shader type. For possible types, see
	 * https://registry.khronos.org/OpenGL-Refpages/gl4/html/glCreateShader.xhtml
	 * @param name Unique name of this shader
	 * @param source Shader source code
	 */
	Shader(int type, const std::string& name, const std::string& source);

	/**
	 * @return The unique name of this shader
	 */
	const std::string getName() const;

	/**
	 * @return The source code for this shader
	 */
	const std::string getSrc() const;

private:

	/**
	 * GLFW id. Will be zero when this shader is not currently mounted.
	 */
	unsigned int _id;

	/**
	 * Shader type
	 */
	unsigned int _type;

	/**
	 * Unique name of this shader
	 */
	std::string _name;

	/**
	 * Source code
	 */
	std::string _src;

	/**
	 * Initializes and compiles this shader. Does nothing if this shader has already been compiled.
	 * This creates the resources necessary to link this shader as part of one or more shader programs.
	 * @throws InstantiationException on failure to compile the shader
	 */
	void mount();

	/**
	 * Releases all resources for this shader. After calling this method, this shader may not be linked
	 * as part of a shader program unless it is re-mounted.
	 */
	void unmount();

	/**
	 * Assignment operator for copying data from another shader
	 * @param shader Shader to copy data from
	 */
	void operator=(const Shader& shader);
};