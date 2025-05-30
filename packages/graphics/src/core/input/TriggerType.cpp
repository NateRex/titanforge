#include <graphics/core/input/TriggerType.h>
#include <GLFW/glfw3.h>

TriggerType mapGLFWActionType(int glfwAction)
{
	switch (glfwAction)
	{
		case GLFW_PRESS:	return TriggerType::PRESSED;
		case GLFW_RELEASE:	return TriggerType::RELEASED;
		case GLFW_REPEAT:	return TriggerType::HELD;
		default:			return TriggerType::PRESSED;
	}
}