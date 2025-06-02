#pragma once
#include <graphics/core/input/pointers/InputContextPtr.h>
#include <vector>
#include <unordered_map>
#include <functional>

struct GLFWwindow;
class InputAction;
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
	 * Binds an action to a function that will be called whenever the action is triggered via one of the active
	 * input contexts
	 * @param action Action
	 * @param callback Callback function
	 */
	void bindAction(const InputAction& action, const ActionCallback& callback);

	/**
	 * Adds a new active input context to this controller. In doing so, any action mappings contained within the
	 * context will be enabled.
	 */
	void addContext(const InputContextPtr context);

	/**
	 * Clear all action bindings. In doing so, there will no longer be any callback functions associated with actions.
	 */
	void clearActionBindings();

	/**
	 * Clear all input contexts. In doing so, there will no longer be any action mappings enabled.
	 */
	void clearContexts();

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
	std::unordered_map<InputAction, ActionCallback> _bindings;

	/**
	 * Constructor
	 * @param glfwWindow A pointer to the GLFW window object for which we want to monitor input
	 */
	InputController(GLFWwindow* glfwWindow);
};