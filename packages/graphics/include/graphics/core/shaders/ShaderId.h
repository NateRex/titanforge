#pragma once

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
	 * Shader for drawing vertex normals
	 */
	VERTEX_NORMALS
};