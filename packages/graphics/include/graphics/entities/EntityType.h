#pragma once

/**
 * Enumeration describing types of entities
 * @author Nathaniel Rex
 */
enum class EntityType
{

	/**
	 * A group of one or more child entities. This entity renders nothing on its own, but applies transformations
	 * to its children.
	 */
	GROUP,

	/**
	 * A camera, which is never rendered, but used to determine the view for rendering
	 */
	CAMERA,

	/**
	 * A mesh entity containing triangulated buffered data to be processed by the GPU
	 */
	MESH
};