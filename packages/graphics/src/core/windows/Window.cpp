#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <graphics/core/windows/Window.h>
#include <graphics/core/windows/InputController.h>
#include <graphics/core/shaders/ShaderManager.h>
#include <graphics/textures/TextureLoader.h>
#include <common/exceptions/InstantiationException.h>
#include <sstream>

bool Window::_HEADLESS = false;
int Window::_WINDOW_COUNT = 0;
std::mutex Window::_MUTEX;

Window::Window(const char* title, unsigned int width, unsigned int height)
{
    incrementWindowCount();

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

    decrementWindowCount();
}

void Window::setHeadlessMode(bool headlessMode)
{
    _HEADLESS = headlessMode;
}

WindowPtr Window::create(const char* title, unsigned int width, unsigned int height)
{
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

void Window::incrementWindowCount()
{
    std::lock_guard<std::mutex> lock(_MUTEX);
    if (_WINDOW_COUNT == 0)
    {
        if (!initGLFW())
        {
            throw InstantiationException("Failed to initialize GLFW");
        }
    }

    _WINDOW_COUNT++;
}

void Window::decrementWindowCount()
{
    std::lock_guard<std::mutex> lock(_MUTEX);
    if (--_WINDOW_COUNT == 0)
    {
        terminateGLFW();
    }
}

bool Window::initGLFW()
{
    if (!glfwInit())
    {
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Additional settings for Apple devices
    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    // Possibly run in headless mode (for test environments)
    if (_HEADLESS)
    {
        glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
    }

    return true;
}

void Window::terminateGLFW()
{
    // Reset globals back to their original state
    ShaderManager::reset();
    TextureLoader::reset();

    glfwTerminate();
}

void Window::onResize(GLFWwindow* window, int width, int height)
{
    if (glfwGetCurrentContext() == window)
    {
        glViewport(0, 0, width, height);
    }
}