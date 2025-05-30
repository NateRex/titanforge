#include <graphics/core/input/InputController.h>
#include <GLFW/glfw3.h>
#include <common/Assertions.h>

InputController::InputController(GLFWwindow* glfwWindow) : _glfwWindow(glfwWindow)
{
	assertNotNull(_glfwWindow, "Failed to create GLFW window");
}

InputController::~InputController()
{

}