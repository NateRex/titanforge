#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <graphics/core/windows/Window.h>
#include <graphics/core/windows/InputController.h>
#include <common/exceptions/InstantiationException.h>
#include <sstream>

Window::Window(const std::string& name)
    : _glfwWindow(nullptr), name(name), _clearColor(Color(0.f, 0.f, 0.f, 0.f))
{

}

void Window::create(unsigned int width, unsigned int height)
{
    // Create window
    _glfwWindow = glfwCreateWindow(800, 600, "TitanForge", NULL, NULL);
    if (!_glfwWindow)
    {
        std::ostringstream oss;
        oss << "Could not create window: " << name;
        throw InstantiationException(oss.str());
    }

    // Create the input controller
    _inputController = std::shared_ptr<InputController>(new InputController(_glfwWindow));

    // Set resize callback
    glfwSetFramebufferSizeCallback(_glfwWindow, onResize);
}

void Window::destroy()
{
    if (glfwGetCurrentContext() == _glfwWindow)
    {
        glfwMakeContextCurrent(nullptr);
    }

    glfwDestroyWindow(_glfwWindow);
    _glfwWindow = nullptr;
}

InputController* Window::getInputController()
{
    return _inputController.get();
}

bool Window::isOpen() const
{
    return !glfwWindowShouldClose(_glfwWindow);
}

void Window::setBackgroundColor(const Color color)
{
    _clearColor = color;
}

void Window::onResize(GLFWwindow* window, int width, int height)
{
    if (glfwGetCurrentContext() == window)
    {
        glViewport(0, 0, width, height);
    }
}