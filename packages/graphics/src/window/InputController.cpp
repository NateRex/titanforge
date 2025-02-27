#include <graphics/window/InputController.h>
#include <GLFW/glfw3.h>

InputController::InputController(GLFWwindow* glfwWindow) : _glfwWindow(glfwWindow)
{

}

InputController::~InputController()
{
	_listeners.clear();
}

bool InputController::isKeyPressed(unsigned int key) const
{
	return glfwGetKey(_glfwWindow, key) == GLFW_PRESS;
}

void InputController::addListener(unsigned int key, std::function<void()> callback)
{
	_listeners.try_emplace(key, std::vector<std::function<void()>>());
	_listeners[key].push_back(callback);
}

void InputController::processInput() const
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