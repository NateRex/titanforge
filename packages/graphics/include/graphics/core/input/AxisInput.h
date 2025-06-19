#pragma once

/**
 * Enumerated type defining a platform-independent set of axis inputs (e.g. - mouse move or mouse scroll).
 * It is used throughout the engine to handle input in an abstract way, allowing for backend-specific
 * mapping (e.g., GLFW, SDL, etc.) without coupling user code to a specific library.
 * @author Nathaniel Rex
 */
enum class AxisInput
{
	MOUSE_SCROLL_2D,
	MOUSE_SCROLL_X,
	MOUSE_SCROLL_Y,
	MOUSE_MOVE
}