#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <graphics/core/windows/Window.h>
#include <graphics/core/windows/InputController.h>
#include <common/exceptions/InstantiationException.h>
#include <sstream>

bool Window::GLFW_INIT = false;
bool Window::HEADLESS = false;

Window::Window(const char* title, unsigned int width, unsigned int height)
{
    // Create window
    _glfwWindow = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!_glfwWindow)
    {
        std::ostringstream oss;
        oss << "Could not create window: " << title;
        throw InstantiationException(oss.str());
    }

    // Create the input controller
    _inputController = new InputController(_glfwWindow);

    // Set resize callback
    glfwSetFramebufferSizeCallback(_glfwWindow, onResize);
}

Window::~Window()
{
    if (glfwGetCurrentContext() == _glfwWindow)
    {
        glfwMakeContextCurrent(nullptr);
    }

    glfwDestroyWindow(_glfwWindow);
    _glfwWindow = nullptr;

    delete _inputController;
    _inputController = nullptr;
}

void Window::setHeadlessMode(bool headlessMode)
{
    HEADLESS = headlessMode;
}

WindowPtr Window::create(const char* title, unsigned int width, unsigned int height)
{
    if (!GLFW_INIT)
    {
        initGLFW();
    }

    return std::shared_ptr<Window>(new Window(title, width, height));
}

InputController* Window::getInputController()
{
    return _inputController;
}

bool Window::isOpen() const
{
    return !glfwWindowShouldClose(_glfwWindow);
}

void Window::initGLFW()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Additional settings for Apple devices
    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    // Possibly run in headless mode (for test environments)
    if (HEADLESS)
    {
        glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
    }

    GLFW_INIT = true;
}

void Window::onResize(GLFWwindow* window, int width, int height)
{
    if (glfwGetCurrentContext() == window)
    {
        glViewport(0, 0, width, height);
    }
}