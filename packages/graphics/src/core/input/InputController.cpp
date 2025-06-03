#include <graphics/core/input/InputController.h>
#include <graphics/core/input/InputContext.h>
#include <graphics/core/input/InputActionMapping.h>
#include <graphics/core/input/InputTrigger.h>
#include <GLFW/glfw3.h>
#include <common/Assertions.h>
#include <algorithm>

InputController::InputController(GLFWwindow* glfwWindow) : _glfwWindow(glfwWindow)
{
	assertNotNull(_glfwWindow, "Input controller requires valid GLFW window");
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

void InputController::processKeyEvent(int glfwKey, int glfwAction, int mods) const
{
	InputKey key = mapGLFWKey(glfwKey);
	InputTrigger trigger = mapGLFWAction(glfwAction);

	std::vector<InputActionMapping> contextMappings;

	// Iterate over contexts
	for (const auto& context : _contexts)
	{
		contextMappings.clear();
		context->get(key, trigger, contextMappings);

		// Iterate over actions mapped to (key, trigger) pair
		for (const auto& mapping : contextMappings)
		{

			// Apply callback if action is bound
			auto binding = _bindings.find(mapping.action);
			if (binding != _bindings.end() && binding->second)
			{
				binding->second(mapping.value);
			}
		}
	}
}