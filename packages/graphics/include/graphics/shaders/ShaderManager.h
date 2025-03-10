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

	/**
	 * Sets the value for a uniform variable located in a shader program. This method will cause that
	 * shader program to become active if it wasn't already.
	 * @param programName The name of shader program containing the variable
	 * @param variableName The name of the uniform variable
	 * @param setFunc GLFW function used to set the variable. This is type specific. For a full list of
	 * the methods available, see https://registry.khronos.org/OpenGL-Refpages/gl4/html/glUniform.xhtml
	 * @param values Values to apply to the uniform variable
	 */
	template <typename Func, typename ...Args>
	static void setUniform(const std::string& programName, const std::string& variableName, Func setFunc, Args... args)
	{
		useProgram(programName);
		int variableLoc = getUniformLocation(programName, variableName);
		setFunc(variableLoc, args...);
	}

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

	/**
	 * Asserts that a shader program with the given name exists within this manager
	 * @param programName The name of the program to search for
	 * @return The shader program, if it exists
	 * @throws IllegalArgumentException if the program does not exist
	 */
	static ShaderProgram assertProgramExists(const std::string& programName);

	/**
	 * Fetches the uniform variable location within a shader program stored by this manager
	 * @param programName The name of the shader program
	 * @param variableName The name of the uniform variable
	 * @return The variable location
	 * @throws IllegalArgumentException if either the program or uniform variable does not exist
	 */
	static int getUniformLocation(const std::string& programName, const std::string& variableName);
};