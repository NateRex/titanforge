#include <graphics/core/input/InputComponent.h>
#include <GLFW/glfw3.h>
#include <common/Assertions.h>

InputComponent::InputComponent(GLFWwindow* glfwWindow) : _glfwWindow(glfwWindow)
{
	assertNotNull(_glfwWindow, "Input component requires valid GLFW window");
}
