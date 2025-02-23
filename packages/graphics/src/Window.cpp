#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <graphics/Window.h>
#include <common/Assertions.h>

Window::Window()
{
    // Initialize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    // Create window
    _glfwWindow = glfwCreateWindow(800, 600, "TitanForge", NULL, NULL);
    assertNotNull(_glfwWindow);
}