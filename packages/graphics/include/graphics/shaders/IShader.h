#pragma once
#include <string>

/**
 * Shader interface
 * @author Nathaniel Rex
 */
class IShader
{
public:

	friend class ShaderManager;

	/**
	 * @return The unique name of this shader
	 */
	const std::string getName() const;

	/**
	 * @return The source code for this shader
	 */
	const std::string getSrc() const;

protected:

	/**
	 * Constructor
	 * @param name Unique name of this shader
	 * @param source Shader source code
	 */
	IShader(const std::string& name, const std::string& source);

private:

	/**
	 * Unique name of this shader
	 */
	std::string _name;

	/**
	 * Source code
	 */
	std::string _src;

	/**
	 * Assignment operator for copying data from another shader
	 * @param shader Shader to copy data from
	 */
	void operator=(const IShader& shader);
};