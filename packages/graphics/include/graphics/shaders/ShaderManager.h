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
	 */
	static void create(const char* name, const char* vertexShader, const char* fragmentShader);

	/**
	 * Updates the current shader program used for rendering
	 * @param name The name of the program
	 * @throws IllegalArgumentException if a program with that name could not be found
	 */
	static void use(const char* name);

	/**
	 * Destroys a shader program held by this manager, releasing all of its resources
	 * @param name The name of the shader program
	 * @throws IllegalArgumentException if a program with that name could not be found
	 */
	static void destroy(const char* name);

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