#include <graphics/core/input/InputController.h>
#include <graphics/core/input/InputContext.h>
#include <graphics/core/input/InputContextMappings.h>
#include <graphics/core/input/InputValue.h>
#include <common/Assertions.h>
#include <unordered_set>
#include <GLFW/glfw3.h>
#include <algorithm>

InputController::InputController(GLFWwindow* glfwWindow) : _glfwWindow(glfwWindow)
{
	assertNotNull(_glfwWindow, "Input controller requires valid GLFW window");

	glfwSetWindowUserPointer(glfwWindow, this);
	glfwSetKeyCallback(glfwWindow, InputController::processKeyEvent);
	glfwSetCursorPosCallback(glfwWindow, InputController::processMouseMovement);
}

InputController::~InputController()
{
	glfwSetKeyCallback(_glfwWindow, nullptr);
	glfwSetCursorPosCallback(_glfwWindow, nullptr);
	glfwSetWindowUserPointer(_glfwWindow, nullptr);
}

void InputController::bind(const InputAction& action, const ActionCallback& callback)
{
	_bindings[action] = callback;
}

void InputController::addContext(const InputContextPtr context)
{
	if (std::find(_contexts.begin(), _contexts.end(), context) == _contexts.end())
	{
		// Only add if context is not already present
		_contexts.push_back(context);
	}
}

void InputController::clearBindings()
{
	_bindings.clear();
}

void InputController::clearContexts()
{
	_contexts.clear();
}

void InputController::processKeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Grab input controller pointer
	InputController* controller = static_cast<InputController*>(glfwGetWindowUserPointer(window));
	if (!controller)
	{
		return;
	}

	controller->processKeyEvent(key, action, mods);
}

void InputController::processKeyEvent(int glfwKey, int glfwAction, int mods) const
{
	DigitalInput key = glfwToInputKey(glfwKey);
	InputTrigger trigger = glfwToInputTrigger(glfwAction);

	if (trigger == InputTrigger::HELD)
	{
		// HELD triggers are handled via polling for a smoother response
		return;
	}

	std::vector<DigitalMapping> contextMappings;

	// Iterate over contexts
	for (const auto& context : _contexts)
	{
		contextMappings.clear();
		context->getDigitalMappings(key, trigger, contextMappings);

		// Iterate over actions mapped to (key, trigger) pair
		for (const auto& mapping : contextMappings)
		{
			const InputAction& action = mapping.action;

			// Apply callback if action is bound
			auto binding = _bindings.find(action);
			if (binding != _bindings.end() && binding->second)
			{
				binding->second(createValue(mapping), _deltaTime);
			}
		}
	}
}

void InputController::poll(float deltaTime)
{
	_deltaTime = deltaTime;

	std::unordered_map<DigitalInput, InputTrigger> key2State;
	std::vector<DigitalMapping> mappings;

	// Iterate over contexts
	for (const auto& context : _contexts)
	{
		mappings.clear();
		context->getDigitalMappings(mappings);

		// Iterate over bindings
		for (const auto& mapping : mappings)
		{
			// Skip if action is not bound. No reason to poll
			auto binding = _bindings.find(mapping.action);
			if (binding == _bindings.end() || !(binding->second))
			{
				continue;
			}

			// Skip if mapping is not triggered via HELD event
			if (mapping.trigger != InputTrigger::HELD)
			{
				continue;
			}

			// Determine key state (caching it for future reference in this method)
			InputTrigger keyState;
			auto it = key2State.find(mapping.key);
			if (it != key2State.end()) {
				keyState = it->second;
			}
			else {
				int value = glfwGetKey(_glfwWindow, inputKeyToGLFW(mapping.key));
				keyState = glfwToInputTrigger(value);
				key2State[mapping.key] = keyState;
			}

			// Trigger callback
			if (keyState == InputTrigger::PRESSED)
			{
				binding->second(createValue(mapping), deltaTime);
			}
		}
	}
}

void InputController::processMouseMovement(GLFWwindow* window, double xPos, double yPos)
{
	// Grab input controller pointer
	InputController* controller = static_cast<InputController*>(glfwGetWindowUserPointer(window));
	if (!controller)
	{
		return;
	}

	controller->processMouseMovement(xPos, yPos);
}

void InputController::processMouseMovement(double xPos, double yPos)
{
	std::vector<AxisMapping> contextMappings;

	float xOffset = xPos - _mouseX;
	float yOffset = yPos - _mouseY;
	_mouseX = xPos;
	_mouseY = yPos;

	// If we haven't yet gotten a starting mouse position, record it and skip over callbacks on this frame
	if (!_didComputeMousePosition)
	{
		_didComputeMousePosition = true;
		return;
	}

	// Iterate over contexts
	for (const auto& context : _contexts)
	{
		contextMappings.clear();
		context->getAxisMappings(AxisInput::MOUSE_MOVE, contextMappings);

		// Iterate over actions mapped to mouse movement
		for (const auto& mapping : contextMappings)
		{
			const InputAction& action = mapping.action;

			// Apply callback if action is bound
			auto binding = _bindings.find(action);
			if (binding != _bindings.end() && binding->second)
			{
				binding->second(createValue(mapping, -xOffset * 0.1, yOffset * 0.1, 0.f), _deltaTime);
			}
		}
	}
}

InputValue InputController::createValue(const DigitalMapping& mapping)
{
	// Start w/ 3D type so that all 3 component values are preserved when applying modifiers
	InputValue value(InputValueType::VECTOR_3D, 1.f, 0.f, 0.f);
	value = mapping.modifiers.apply(value);
	Vector3 vec = value.get3D();

	// Repack values into input with correct type
	return InputValue(mapping.action.getValueType(), vec.x, vec.y, vec.z);
}

InputValue InputController::createValue(const AxisMapping& mapping, float x, float y, float z)
{
	// Start w/ 3D type so taht all 3 component values are preserved when applying modifiers
	InputValue value(InputValueType::VECTOR_3D, 1.f, 0.f, 0.f);
	value = mapping.modifiers.apply(value);
	Vector3 vec = value.get3D();

	// Repack values into input with correct type
	return InputValue(mapping.action.getValueType(), vec.x, vec.y, vec.z);
}