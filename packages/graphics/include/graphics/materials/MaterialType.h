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
	 * Material applied to line primitives
	 */
	LINE,

	/**
	 * Material applied to meshes
	 */
	MESH,

	/**
	 * Material used to render wireframe representations of a mesh
	 */
	WIREFRAME,

	/**
	 * Material applied to skyboxes
	 */
	SKYBOX,

	/**
	 * Full-screen post-processing material
	 */
	POST_PROCESS
};
