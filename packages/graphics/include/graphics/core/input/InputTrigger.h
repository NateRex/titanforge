#pragma once

/**
 * Enumerated type used to distinguish between different types of key or button events.
 * It allows the input system to respond appropriately based on whether a key is pressed,
 * released, or held down.
 * @author Nathaniel Rex
 */
enum class InputTrigger
{
	/**
	 * Input is pressed (e.g., a key or mouse down event)
	 */
	PRESSED,

	/**
	 * Input is released (e.g., a key or mouse up event)
	 */
	RELEASED,

	/**
	 * Input is continuously held down
	 */
	HELD
};

/**
 * Maps a GLFW action type to an input trigger type
 * @param glfwAction Integer value representing a GLFW action type
 * @return The corresponding input trigger type
 */
InputTrigger mapGLFWAction(int glfwAction);