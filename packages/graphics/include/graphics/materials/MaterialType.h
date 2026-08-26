#pragma once

/**
 * Enumeration describing types of materials
 * @author Nathaniel Rex
 */
enum class MaterialType
{

	/**
	 * Material applied to point primitives
	 */
	POINT,

	/**
	 * Material applicable to meshes
	 */
	MESH,

	/**
	 * Skybox material
	 */
	SKYBOX,

	/**
	 * Full-screen post-processing material
	 */
	POST_PROCESS
};
