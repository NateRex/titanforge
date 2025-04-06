#pragma once
#include <graphics/core/windows/pointers/WindowPtr.h>

/**
 * The renderer is responsible for managing the current render context target and drawing the scene
 * @author Nathaniel Rex
 */
class Renderer
{
public:

	/**
	 * Constructs a new renderer instance. This will automatically result in a new window being created and made the
	 * current context.
	 */
	Renderer();

	/**
	 * Constructs a new renderer instance using an existing window
	 * @param window Window target
	 */


private:

	/**
	 * 
	 */

	/**
	 * The current window context
	 */
	WindowPtr _currentWindow;

	/**
	 * The current 
	 */
};