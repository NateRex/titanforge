#pragma once
#include <map>
#include <functional>
#include <vector>

struct GLFWwindow;

/**
 * An input controller is created upon creation of a window, and handles all user mouse and keyboard
 * events while that window is active.
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

	/**
	 * Overrides the state of a key to a constant value. This value will be used over actual
	 * system state when evaluating key presses.
	 * @param key The key of interest. For a mapping of key name to integer values, see
	 * https://www.glfw.org/docs/3.3/group__keys.html
	 * @param state GLFW key state (either GLFW_PRESS or GLFW_RELEASE).
	 */
	void setOverride(int key, int state);

	/**
	 * Removes an override previously set via setOverride()
	 * @param key The key of interest. For a mapping of key name to integer values, see
	 * https://www.glfw.org/docs/3.3/group__keys.html
	 */
	void removeOverride(int key);

	/**
	 * Determines if the given key is currently pressed
	 * @param key The key of interest. For a mapping of key name to integer values, see
	 * https://www.glfw.org/docs/3.3/group__keys.html
	 * @return True if that key is currently pressed. Returns false otherwise.
	 */
	bool isKeyPressed(int key);

	/**
	 * Registers a new callback function that will be invoked for a given key
	 * @param key The key of interest. For a mapping of key name to integer values, see
	 * https://www.glfw.org/docs/3.3/group__keys.html
	 * @param callback Callback function to be invoked when the key is pressed
	 */
	void addListener(int key, std::function<void()> callback);

	/**
	 * Processes all current inputs and invokes callback functions for any keys pressed
	 */
	void processInput();

private:

	/**
	 * A pointer to the GLFW window object
	 */
	GLFWwindow* _glfwWindow;

	/**
	 * Overrides for key states. Values in this map will be used over actual system state when
	 * present.
	 */
	std::map<int, bool> _stateOverrides;

	/**
	 * A mapping from key values to callback functions that should be triggered when those
	 * keys are pressed
	 */
	std::map<int, std::vector<std::function<void()>>> _listeners;

	/**
	 * Constructor
	 * @param glfwWindow A pointer to the GLFW window object for which we want to monitor input
	 */
	InputController(GLFWwindow* glfwWindow);
};