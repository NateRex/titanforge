#pragma once
#include <unordered_map>

struct GLFWwindow;

/**
 * This class provides a flexible input system that allows mapping raw key inputs
 * to named actions (e.g., "Jump", "Shoot"), which are then bound to command objects
 * (e.g., entity methods or behaviors). It is intended to decouple input handling from gameplay
 * logic using the Command Pattern.
 * 
 * @author Nathaniel Rex
 */
class InputController
{
public:
	
	friend class Window;

	/**
	 * Destructor
	 */
	~InputController();


private:

	/**
	 * A pointer to the GLFW window object
	 */
	GLFWwindow* _glfwWindow;

	/**
	 * A mapping from GLFW key values to 
	 */

	/**
	 * Constructor
	 * @param glfwWindow A pointer to the GLFW window object for which we want to monitor input
	 */
	InputController(GLFWwindow* glfwWindow);
};