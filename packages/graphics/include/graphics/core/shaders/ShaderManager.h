#pragma once
#include <graphics/core/shaders/pointers/ShaderPtr.h>
#include <unordered_map>

enum class MaterialType;

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
	 * Fetches a shader program for use against a given material type
	 * @param matType Material type
	 * @return The shader program capable of handling that material
	 * @throws NullPointerException If a shader for that material type could not be found
	 */
	static ShaderPtr getShader(MaterialType matType);

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
	 * Mapping from material types to shaders
	 */
	std::unordered_map<MaterialType, ShaderPtr> _shaders;

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