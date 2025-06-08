#include <graphics/core/input/InputTrigger.h>
#include <GLFW/glfw3.h>

InputTrigger glfwToInputTrigger(int glfwAction)
{
	switch (glfwAction)
	{
		case GLFW_PRESS:	return InputTrigger::PRESSED;
		case GLFW_RELEASE:	return InputTrigger::RELEASED;
		case GLFW_REPEAT:	return InputTrigger::HELD;
		default:			return InputTrigger::RELEASED;
	}
}