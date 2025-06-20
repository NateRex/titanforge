#pragma once
#include <graphics/core/input/pointers/InputContextPtr.h>
#include <graphics/core/input/InputAction.h>
#include <graphics/core/input/InputValue.h>
#include <vector>
#include <unordered_map>
#include <functional>

struct GLFWwindow;
struct DigitalMapping;
struct AxisMapping;

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
	 * Processes a key event by evaluating what (if any) actions are mapped to that key via the active input contexts,
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
	void pollForKeyHolds(float deltaTime);

	/**
	 * Processes mouse movement events by evaluating what (if any) actions are mapped to mouse movement via the active input
	 * contexts, and executing callbacks bound to those actions.
	 * @param xPos X position of the mouse (in window coordinates)
	 * @param yPos Y position of the mouse (in window coordinates)
	 */
	void processMouseMovement(double xPos, double yPos);

	/**
	 * Processes mouse scroll events by evaluating what (if any) actions are mapped to mouse movement via the active input
	 * contexts, and executing callbacks bound to those actions.
	 * @param glfwWindow GLFW window pointer
	 * @param xOffset Scroll delta in the horizontal direction
	 * @param yOffset Scroll delta in the vertical direction
	 */
	void processMouseScroll(double xOffset, double yOffset);

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
	 * The x position of the cursor in the previous frame (in pixels). This value is set once per frame, and is used to determine
	 * the offset of mouse movement.
	 */
	float _mouseX = 0.f;

	/**
	 * The y position of the cursor in the previous frame (in pixels). This value is set once per frame, and is used to determine
	 * the offset of mouse movement.
	 */
	float _mouseY = 0.f;

	/**
	 * Boolean flag that, when true, implies that this controller has computed a starting mouse position, and thus mouse movement events
	 * are active.
	 */
	bool _didComputeMousePosition = false;

	/**
	 * Constructor
	 * @param glfwWindow A pointer to the GLFW window object for which we want to monitor input
	 */
	InputController(GLFWwindow* glfwWindow);

	/**
	 * Processes a key events by evaluating what (if any) actions are mapped to that key via the input controller
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
	 * Processes mouse movement events by evaluating what (if any) actions are mapped to mouse movement via the input
	 * controller associated with the window.
	 * @param glfwWindow GLFW window pointer
	 * @param xPos X position of the mouse (in window coordinates)
	 * @param yPos Y position of the mouse (in window coordinates)
	 */
	static void processMouseMovement(GLFWwindow* window, double xPos, double yPos);

	/**
	 * Processes mouse scroll events by evaluating what (if any) actions are mapped to mouse scrolling via the input
	 * controller associated with the window.
	 * @param glfwWindow GLFW window pointer
	 * @param xOffset Scroll delta in the horizontal direction
	 * @param yOffset Scroll delta in the vertical direction
	 */
	static void processMouseScroll(GLFWwindow* window, double xOffset, double yOffset);

	/**
	 * Creates an input value object for a given digital input action mapping
	 * @param mapping Input action mapping for a digital (key, trigger) pair (e.g., a key press)
	 * @return The input value result
	 */
	static InputValue createValue(const DigitalMapping& mapping);

	/**
	 * Creates an input value object for a given axis action mapping
	 * @param mapping Action mapping for an axis input (e.g., mouse move or mouse scroll)
	 * @param x X coordinate value
	 * @param y Y coordinate value
	 * @param z Z coordinate value
	 * @return The input value result
	 */
	static InputValue createValue(const AxisMapping& mapping, float x, float y, float z);
};