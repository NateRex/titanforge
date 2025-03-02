#pragma once
#include <map>
#include <string>
#include <mutex>

class Shader;

/**
 * The shader manager is responsible for tracking all shaders that have been registered
 * for use with the graphics engine
 * @author Nathaniel Rex
 */
class ShaderManager
{
public:

	friend class Engine;

	/**
	 * Compiles and registers a new shader for use by the graphics engine
	 * @param shader Shader instance
	 */
	static void loadShader(Shader& shader);

	/**
	 * Fetches a shader by name
	 * @param name The name of the shader
	 * @return A pointer to the shader, or null if not found
	 */
	static const Shader* get(const std::string name);

private:
	
	/**
	 * Mutex lock for accessing the global instance
	 */
	static std::mutex _MUTEX;

	/**
	 * Map containing all loaded shaders, referenced by name
	 */
	static std::map<std::string, Shader> _SHADERS;

	/**
	 * Constructor
	 */
	ShaderManager() = delete;

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
	 * Compiles and registers all built-in shaders for use by the graphics engine
	 */
	static void loadDefaults();

	/**
	 * De-registers and clears this manager of all shaders
	 */
	static void clear();
};