#pragma once
#include <graphics/core/shaders/pointers/ShaderPtr.h>
#include <unordered_map>

/**
 * Identifies a concrete shader program managed by ShaderManager.
 *
 * ShaderId deliberately remains separate from MaterialType. Whereas a material describes
 * an object's appearance, a shader represents one rendering implementation. In this way, we do not enforce
 * a one-to-one relationship between shaders and materials.
 * 
 * @author Nathaniel Rex
 */
enum class ShaderId
{
	/**
	 * Shader for handling point primitives
	 */
	POINT,

	/**
	 * Shader for handling line segments and polylines
	 */
	LINE,

	/**
	 * Shader for handling mesh surfaces
	 */
	MESH,

	/**
	 * Shader for render mesh edges as a wireframe
	 */
	WIREFRAME,

	/**
	 * Shader for handling skyboxes
	 */
	SKYBOX,

	/**
	 * Shader for handling post-processing effects
	 */
	POST_PROCESS,

	/**
	 * Shader for drawing surface normals
	 */
	NORMALS
};

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
