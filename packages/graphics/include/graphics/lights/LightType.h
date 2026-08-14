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
	 * A positioned light that illuminates radially in all directions
	 */
	POINT,

	/**
	 * A light that extends infinitely in a direction
	 */
	DIRECTIONAL,

	/**
	 * A positioned light that extends as a cone in a direction
	 */
	SPOTLIGHT
};