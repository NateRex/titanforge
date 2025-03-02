#pragma once
#include <string>

/**
 * A shader capable of being loaded onto the GPU
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
	 * GLFW id. Will not be valid until this shader has been loaded.
	 */
	unsigned int _id;

	/**
	 * True if this shader has already been successfully loaded. False otherwise.
	 */
	bool _loaded;

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
	 * Loads and compiles this shader for use on the GPU
	 * @throws InstantiationException if the shader compilation fails
	 */
	void load();

	/**
	 * Assignment operator for copying data from another shader
	 * @param shader Shader to copy data from
	 */
	void operator=(const Shader& shader);
};