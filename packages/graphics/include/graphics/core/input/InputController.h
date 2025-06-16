#pragma once
#include <graphics/core/input/pointers/InputContextPtr.h>
#include <graphics/core/input/InputAction.h>
#include <vector>
#include <unordered_map>
#include <functional>

struct GLFWwindow;
struct InputActionMapping;
class InputValue;

using ActionCallback = std::function<void(const InputValue&, float deltaTime)> ;

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
	 * and executing callbacks bound to those actions. This event-driven method of resolving inputs is specific to actions bound
	 * to PRESSED or RELEASED triggers. This method is called automatically in response to key presses and releases.
	 * @param glfwKey GLFW key code
	 * @param glfwAction GLFW trigger type
	 * @param mods GLFW modifier bits
	 */
	void processKeyEvent(int glfwKey, int glfwAction, int mods) const;

	/**
	 * Polls for held keys and dispatches callbacks for actions bound to the HELD trigger. This method is called once
	 * per frame (e.g, inside the game loop) by the renderer.
	 * @param deltaTime Time since the last frame renderered (in decimal seconds)
	 */
	void poll(float deltaTime);

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
	 * The time (in decimal seconds) since the last frame. This value is set once per frame by the main application loop
	 * and is available during input processing, including event-driven callbacks such as key presses. It enables time-dependent
	 * behavior (e.g., movement speed) within input callbacks.
	 */
	float _deltaTime = 0.f;

	/**
	 * Constructor
	 * @param glfwWindow A pointer to the GLFW window object for which we want to monitor input
	 */
	InputController(GLFWwindow* glfwWindow);

	/**
	 * Processes the given key event by evaluating what (if any) actions are mapped to that key via the input controller
	 * associated with the window. This event-driven method of resolving inputs is used for actions bound to PRESSED
	 * or RELEASED triggers.
	 * @param glfwWindow GLFW window pointer
	 * @param glfwKey GLFW key code
	 * @param scancode The platform-specific scan code of the key.
	 * @param The GLFW action type (GLFW_PRESS, GLFW_RELEASE, GLFW_REPEAT)
	 * @param mods Bit field describing which modifier keys were held.
	 */
	static void processKeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods);

	/**
	 * Creates an input value object for a given input action mapping
	 * @param mapping Input action mapping
	 * @param x X component value
	 * @param y Y component value
	 * @param z Z component value
	 * @return The input value result
	 */
	static InputValue createValue(const InputActionMapping& mapping, float x, float y, float z);
};