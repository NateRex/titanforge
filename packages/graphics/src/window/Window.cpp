#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <graphics/window/Window.h>
#include <common/Assertions.h>

Window::Window()
    : _clearColor(Color::fromFloats(0.f, 0.f, 0.f, 0.f))
{

    // Create window context
    _glfwWindow = glfwCreateWindow(800, 600, "TitanForge", NULL, NULL);
    assertNotNull(_glfwWindow, "Failed to create GLFW window", []() {
        glfwTerminate();
        });
    glfwMakeContextCurrent(_glfwWindow);

    // Create the input controller
    _inputController = std::unique_ptr<InputController>(new InputController(_glfwWindow));

    // Ensure GLFW functions have been loaded via GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        throw std::runtime_error("Failed to initialize GLAD");
    }

    // Set viewport
    glViewport(0, 0, 800, 600);
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

void Window::renderFrame() const
{
    _inputController->processInput();

    glClearColor(_clearColor.red, _clearColor.green, _clearColor.blue, _clearColor.alpha);
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(_glfwWindow);
    glfwPollEvents();
}

void Window::onResize(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}