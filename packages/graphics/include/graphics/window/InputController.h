#pragma once
#include <map>
#include <vector>
#include <functional>

struct GLFWwindow;

/**
 * An input controller is created upon creation of a window, and handles all user mouse and keyboard
 * events while that window is active.
 * @author Nathaniel Rex
 */
class InputController {
public:

	/**
	 * Constructor
	 * @param glfwWindow A pointer to the GLFW window object for which we want to monitor input
	 */
	InputController(GLFWwindow* glfwWindow);

	/**
	 * Destructor
	 */
	~InputController();

	/**
	 * Determines if the given key is currently pressed
	 * @param key The key of interest. For a mapping of key name to integer values, see
	 * https://www.glfw.org/docs/3.3/group__keys.html
	 * @return True if that key is currently pressed. Returns false otherwise.
	 */
	bool isKeyPressed(unsigned int key) const;

	/**
	 * Registers a new callback function that will be invoked for a given key
	 * @param key The key of interest. For a mapping of key name to integer values, see
	 * https://www.glfw.org/docs/3.3/group__keys.html
	 * @param callback Callback function to be invoked when the key is pressed
	 */
	void addListener(unsigned int key, std::function<void()> callback);

	/**
	 * Processes all current inputs and invokes callback functions for any keys pressed
	 */
	void processInput() const;

private:

	/**
	 * A pointer to the GLFW window object
	 */
	GLFWwindow* _glfwWindow;

	/**
	 * A mapping from key values to callback functions that should be triggered when those
	 * keys are pressed
	 */
	std::map<unsigned int, std::vector<std::function<void()>>> _listeners;
};