#pragma once
#include <iosfwd>
#include <map>
#include <mutex>

class Shader;

/**
 * The shader manager is responsible for tracking all shaders that have been registered for use
 * @author Nathaniel Rex
 */
class ShaderManager
{
public:

	friend class Engine;

	/**
	 * Links and registers a new shader program for use in this manager
	 * @param name Unique name for the shader
	 * @param vertexShader Vertex shader source code
	 * @param fragmentShader Fragment shader source code
	 * @return The new shader program
	 * @throws IllegalArgumentException if a shader with that name already exists
	 * @throws InstantiationException on failure to compile or link the shader
	 */
	static Shader* create(const std::string& name, const char* vertexShader, const char* fragmentShader);

	/**
	 * Fetches a shader program by name
	 * @param name The name of the shader previously created via this manager
	 * @return The shader
	 * @throws IllegalArgumentException if a program with that name could not be found
	 */
	static Shader* get(const std::string& name);

	/**
	 * Destroys a shader program held by this manager, releasing all of its resources
	 * @param name The name of the shader program
	 * @throws IllegalArgumentException if a program with that name could not be found
	 */
	static void destroy(const std::string& name);

private:
	
	/**
	 * Mutex lock for manipulating static data
	 */
	static std::mutex _MUTEX;

	/**
	 * Map containing all shaders that have been created
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
	 * Initializes this shader manager by compiling and linking all default shader programs,
	 * and sets the starting shader program for use by the GPU
	 */
	static void setup();

	/**
	 * Clears and deletes all shaders held by this manager
	 */
	static void clear();
};