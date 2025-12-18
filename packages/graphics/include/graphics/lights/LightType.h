#pragma once

/**
 * Enumeration describing types of lights
 * @author Nathaniel Rex
 */
enum class LightType
{

	/**
	 * Light that globally illuminates all objects in the scene equally
	 */
	AMBIENT,

	/**
	 * A light that illuminates from a point in space
	 */
	POINT
};