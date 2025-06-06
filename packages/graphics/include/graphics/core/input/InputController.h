#pragma once
#include <graphics/core/input/pointers/InputContextPtr.h>
#include <graphics/core/input/InputAction.h>
#include <vector>
#include <unordered_map>
#include <functional>

struct GLFWwindow;
class InputValue;

using ActionCallback = std::function<void(const InputValue&)> ;

/**
 * Central dispatcher for input events based on active contexts and action bindings. This class receives GLFW key
 * events and resolves them through all active input contexts. It then invokes the appropriate callback bound to
 * the matching input action.
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
	 * Adds a new active input context to this controller. In doing so, any action mappings contained within the
	 * context will be enabled.
	 */
	void addContext(const InputContextPtr context);

	/**
	 * Binds an action to a function that will be called whenever the action is triggered via one of the active
	 * input contexts
	 * @param action Action
	 * @param callback Callback function
	 */
	void bind(const InputAction& action, const ActionCallback& callback);

	/**
	 * Clear all input contexts. In doing so, there will no longer be any action mappings enabled.
	 */
	void clearContexts();

	/**
	 * Clear all action bindings. In doing so, there will no longer be any callback functions associated with actions.
	 */
	void clearBindings();

	/**
	 * Processes the given key event by evaluating what (if any) actions are mapped to that key via the active input contexts,
	 * and executing callbacks bound to those actions.
	 * @param glfwKey GLFW key code
	 * @param glfwAction GLFW trigger type
	 * @param mods GLFW modifier bits
	 */
	void processKeyEvent(int glfwKey, int glfwAction, int mods) const;

private:

	/**
	 * A pointer to the GLFW window object
	 */
	GLFWwindow* _glfwWindow;

	/**
	 * The list of all active input contexts
	 */
	std::vector<InputContextPtr> _contexts;

	/**
	 * Mapping from input actions to callback values
	 */
	std::unordered_map<InputAction, ActionCallback, InputAction::Hash> _bindings;

	/**
	 * Constructor
	 * @param glfwWindow A pointer to the GLFW window object for which we want to monitor input
	 */
	InputController(GLFWwindow* glfwWindow);

	/**
	 * Processes the given key event by evaluating what (if any) actions are mapped to that key via the input controller
	 * associated with the window.
	 * @param glfwWindow GLFW window pointer
	 * @param glfwKey GLFW key code
	 * @param scancode The platform-specific scan code of the key.
	 * @param The GLFW action type (GLFW_PRESS, GLFW_RELEASE, GLFW_REPEAT)
	 * @param mods Bit field describing which modifier keys were held.
	 */
	static void processKeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods);
};