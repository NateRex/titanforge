#include <graphics/core/input/InputController.h>
#include <graphics/core/input/InputContext.h>
#include <graphics/core/input/InputActionMapping.h>
#include <graphics/core/input/InputTrigger.h>
#include <common/Assertions.h>
#include <unordered_set>
#include <GLFW/glfw3.h>
#include <algorithm>

InputController::InputController(GLFWwindow* glfwWindow) : _glfwWindow(glfwWindow)
{
	assertNotNull(_glfwWindow, "Input controller requires valid GLFW window");

	glfwSetWindowUserPointer(glfwWindow, this);
	glfwSetKeyCallback(glfwWindow, InputController::processKeyEvent);
}

InputController::~InputController()
{
	glfwSetKeyCallback(_glfwWindow, nullptr);
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
	InputKey key = glfwToInputKey(glfwKey);
	InputTrigger trigger = glfwToInputTrigger(glfwAction);

	if (trigger == InputTrigger::HELD)
	{
		// HELD triggers are handled via polling for a smoother response
		return;
	}

	std::vector<InputActionMapping> contextMappings;

	// Iterate over contexts
	for (const auto& context : _contexts)
	{
		contextMappings.clear();
		context->getMappings(key, trigger, contextMappings);

		// Iterate over actions mapped to (key, trigger) pair
		for (const auto& mapping : contextMappings)
		{

			// Apply callback if action is bound
			auto binding = _bindings.find(mapping.action);
			if (binding != _bindings.end() && binding->second)
			{
				binding->second(mapping.value, _deltaTime);
			}
		}
	}
}

void InputController::poll(float deltaTime)
{
	_deltaTime = deltaTime;

	std::unordered_map<InputKey, InputTrigger> key2State;
	std::vector<InputActionMapping> mappings;

	// Iterate over contexts
	for (const auto& context : _contexts)
	{
		mappings.clear();
		context->getMappings(mappings);

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
				binding->second(mapping.value, deltaTime);
			}
		}
	}
}