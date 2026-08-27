#pragma once
#include <graphics/core/shaders/pointers/ShaderPtr.h>
#include <graphics/core/shaders/ShaderId.h>
#include <unordered_map>

/**
 * The shader manager is a singleton, responsible for tracking all shaders that have been registered for use
 * @author Nathaniel Rex
 */
class ShaderManager
{
public:

	/**
	 * Destructor
	 */
	~ShaderManager();

	/**
	 * Fetches a registered shader program.
	 * @param id Shader identifier
	 * @return The requested shader program
	 * @throws NullPointerException If the shader could not be found
	 */
	static ShaderPtr getShader(ShaderId id);

	/**
	 * Resets the global shader manager instance to its initial state, prior to graphics initialization
	 */
	static void reset();

private:

	/**
	 * Global shader manager instance
	 */
	static std::unique_ptr<ShaderManager> _INSTANCE;

	/**
	 * Mapping from shader identifiers to programs
	 */
	std::unordered_map<ShaderId, ShaderPtr> _shaders;

	/**
	 * Constructor
	 */
	ShaderManager();

	/**
	 * Constructor
	 * @param mgr Shader manager to copy from
	 */
	ShaderManager(const ShaderManager& mgr) = delete;

	/**
	 * Constructor
	 * @param mgr Shader manager to copy from
	 */
	ShaderManager(ShaderManager&& mgr) = delete;

	/**
	 * Assignment operator
	 * @param mgr Shader manager to assign from
	 */
	ShaderManager& operator=(const ShaderManager& mgr) = delete;

	/**
	 * @return The global ShaderManager instance
	 */
	static ShaderManager* getInstance();
};
