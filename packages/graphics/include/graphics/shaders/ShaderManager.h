#pragma once
#include <iosfwd>
#include <map>
#include <mutex>

class Shader;
class ShaderProgram;


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
	 * Mounts and compiles a new shader for use in linking
	 * @param shader Shader
	 */
	static void mountShader(Shader& shader);

	/**
	 * Unmounts all shaders
	 */
	static void unmountShaders();

	/**
	 * Links and stores a new shader program for use via the one or more of the shaders currently mounted.
	 * @param prgm The shader program
	 */
	static void linkProgram(ShaderProgram& prgm);

	/**
	 * Updates the shader program used for rendering
	 * @param name The name of the program
	 */
	static void useProgram(const std::string& name);

private:
	
	/**
	 * Mutex lock for manipulating static data
	 */
	static std::mutex _MUTEX;

	/**
	 * Map containing all shaders currently mounted and ready for linking
	 */
	static std::map<std::string, Shader> _SHADERS;

	/**
	 * Map containing all shader programs that have been linked and are ready-for-use
	 */
	static std::map<std::string, ShaderProgram> _PROGRAMS;

	/**
	 * The name of the currently-active shader program.
	 */
	static std::string _ACTIVE;

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
	 * Unmounts all shaders, and deletes all previously-created shader programs
	 */
	static void clear();
};