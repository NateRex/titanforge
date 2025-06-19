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
	 * Key is pressed (e.g., a key or mouse down event)
	 */
	PRESSED,

	/**
	 * Key is released (e.g., a key or mouse up event)
	 */
	RELEASED,

	/**
	 * Key is continuously held down
	 */
	HELD,

	/**
	 * Movement of device (e.g., mouse move)
	 */
	MOVE
};

/**
 * Maps a GLFW action type to an input trigger type
 * @param glfwAction Integer value representing a GLFW action type
 * @return The corresponding input trigger type
 */
InputTrigger glfwToInputTrigger(int glfwAction);