#pragma once

/**
 * Describes the type of value that an input action expects
 * @author Nathaniel Rex
 */
enum class InputValueType
{

	/**
	 * True or false values
	 */
	BOOLEAN,

	/**
	 * Single float value (e.g. throttle)
	 */
	SCALAR,

	/**
	 * Two-dimensional vector (e.g. movement)
	 */
	VECTOR_2D,

	/**
	 * Three-dimensional vector (e.g. VR controller)
	 */
	VECTOR_3D
};