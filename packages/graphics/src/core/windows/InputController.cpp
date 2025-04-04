#include <graphics/core/windows/InputController.h>
#include <GLFW/glfw3.h>
#include <common/Assertions.h>

InputController::InputController(GLFWwindow* glfwWindow) : _glfwWindow(glfwWindow)
{
	assertNotNull(_glfwWindow, "Failed to create GLFW window");
}

InputController::~InputController()
{
	_listeners.clear();
}

void InputController::setOverride(int key, int state)
{
	_stateOverrides[key] = state == GLFW_PRESS;
}

void InputController::removeOverride(int key)
{
	_stateOverrides.erase(key);
}

bool InputController::isKeyPressed(int key)
{
	if (_stateOverrides.find(key) != _stateOverrides.end())
	{
		return _stateOverrides[key];
	}

	return glfwGetKey(_glfwWindow, key) == GLFW_PRESS;
}

void InputController::addListener(int key, std::function<void()> callback)
{
	_listeners.try_emplace(key, std::vector<std::function<void()>>());
	_listeners[key].push_back(callback);
}

void InputController::processInput()
{
	for (const auto& [key, listeners] : _listeners)
	{
		if (isKeyPressed(key))
		{
			for (const auto& callback : listeners)
			{
				callback();
			}
		}
	}
}