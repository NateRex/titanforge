#include <graphics/core/input/InputEventType.h>
#include <GLFW/glfw3.h>

InputEventType mapGLFWActionType(int glfwAction)
{
	switch (glfwAction)
	{
		case GLFW_PRESS:	return InputEventType::PRESSED;
		case GLFW_RELEASE:	return InputEventType::RELEASED;
		case GLFW_REPEAT:	return InputEventType::HELD;
		default:			return InputEventType::PRESSED;
	}
}