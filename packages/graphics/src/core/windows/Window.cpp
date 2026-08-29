#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <graphics/core/windows/Window.h>
#include <graphics/core/input/InputController.h>
#include <common/exceptions/InstantiationException.h>
#include <sstream>

bool Window::_HEADLESS = false;
int Window::_WINDOW_COUNT = 0;
std::mutex Window::_MUTEX;

Window::Window(const char* title, unsigned int width, unsigned int height, WindowFlags windowFlags) :
    _glfwWindow(nullptr),
    _windowFlags(windowFlags),
    _inputController(nullptr)
{
    incrementWindowCount();

    // Create window
    GLFWmonitor* monitor = hasFlag(_windowFlags, WindowFlags::FULLSCREEN) ? glfwGetPrimaryMonitor() : nullptr;
    _glfwWindow = glfwCreateWindow(width, height, title, monitor, nullptr);
    if (!_glfwWindow)
    {
        decrementWindowCount();
        std::ostringstream oss;
        oss << "Could not create window: " << title;
        throw InstantiationException(oss.str());
    }

    // Vsync is context-specific and must be configured while this window's context is current.
    GLFWwindow* previousContext = glfwGetCurrentContext();
    glfwMakeContextCurrent(_glfwWindow);
    glfwSwapInterval(hasFlag(_windowFlags, WindowFlags::VSYNC) ? 1 : 0);
    glfwMakeContextCurrent(previousContext);

    // Create the input controller
    _inputController = new InputController(_glfwWindow);

    // Set resize callback
    glfwSetFramebufferSizeCallback(_glfwWindow, onResize);
}

Window::~Window()
{
    destroy();
}

void Window::setHeadlessMode(bool headlessMode)
{
    _HEADLESS = headlessMode;
}

WindowPtr Window::create(const char* title, unsigned int width, unsigned int height, WindowFlags windowFlags)
{
    return std::shared_ptr<Window>(new Window(title, width, height, windowFlags));
}

void Window::getDimensions(int* width, int* height) const
{
	if (_glfwWindow)
	{
		glfwGetFramebufferSize(_glfwWindow, width, height);
	}
	else
    {
        *width = 0;
        *height = 0;
    }
}

InputController* Window::getInputController()
{
    return _inputController;
}

bool Window::isOpen() const
{
    return _glfwWindow && !glfwWindowShouldClose(_glfwWindow);
}

void Window::close()
{
    if (_glfwWindow)
    {
        glfwSetWindowShouldClose(_glfwWindow, GLFW_TRUE);
    }
}

void Window::destroy()
{
    if (glfwGetCurrentContext() == _glfwWindow)
    {
        glfwMakeContextCurrent(nullptr);
    }

    if (_inputController)
    {
        delete _inputController;
        _inputController = nullptr;
    }

    if (_glfwWindow)
    {
        glfwDestroyWindow(_glfwWindow);
        _glfwWindow = nullptr;

        decrementWindowCount();
    }
}

void Window::incrementWindowCount()
{
    std::lock_guard<std::mutex> lock(_MUTEX);
    if (!initGLFW())
    {
        throw InstantiationException("Failed to initialize GLFW");
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
    if (_WINDOW_COUNT == 0 && !glfwInit())
    {
        return false;
    }

    // Window hints persist globally in GLFW. Reset them so each window is configured only
    // from its own flags, including when multiple windows use different settings.
    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_STENCIL_BITS, 8);
    glfwWindowHint(GLFW_RESIZABLE, hasFlag(_windowFlags, WindowFlags::RESIZABLE) ? GLFW_TRUE : GLFW_FALSE);
    glfwWindowHint(GLFW_SAMPLES, hasFlag(_windowFlags, WindowFlags::ANTI_ALIASING) ? 4 : 0);

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
    glfwTerminate();
}

void Window::onResize(GLFWwindow* window, int width, int height)
{
    if (glfwGetCurrentContext() == window)
    {
        glViewport(0, 0, width, height);
    }
}
