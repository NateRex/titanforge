#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <graphics/window/Window.h>
#include <graphics/window/InputController.h>
#include <common/Assertions.h>

Window::Window()
    : _clearColor(Color::fromFloats(0.f, 0.f, 0.f, 0.f))
{
    // Create window
    _glfwWindow = glfwCreateWindow(800, 600, "TitanForge", NULL, NULL);
    assertNotNull(_glfwWindow, "Failed to create GLFW window");

    // Create the input controller
    _inputController = std::shared_ptr<InputController>(new InputController(_glfwWindow));

    // Set resize callback
    glfwSetFramebufferSizeCallback(_glfwWindow, onResize);
}

InputController* Window::getInputController()
{
    return _inputController.get();
}

bool Window::isOpen() const
{
    return !glfwWindowShouldClose(_glfwWindow);
}

void Window::close()
{
    glfwSetWindowShouldClose(_glfwWindow, true);
}

void Window::setBackgroundColor(const Color color)
{
    _clearColor = color;
}

void Window::makeCurrent()
{
    glfwMakeContextCurrent(_glfwWindow);

    int width, height;
    glfwGetWindowSize(_glfwWindow, &width, &height);
    glViewport(0, 0, width, height);
}

void Window::onResize(GLFWwindow* window, int width, int height)
{
    if (glfwGetCurrentContext() == window)
    {
        glViewport(0, 0, width, height);
    }
}