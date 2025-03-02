#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <graphics/Engine.h>
#include <graphics/window/Window.h>
#include <graphics/Buffer.h>
#include <graphics/shaders/ShaderManager.h>
#include <graphics/shaders/Shader.h>
#include <common/exceptions/IllegalStateException.h>

std::mutex Engine::_MUTEX;
Engine* Engine::_ENGINE = nullptr;

Engine::Engine()
{

}

Window Engine::start(bool headlessMode)
{
    std::lock_guard<std::mutex> lock(_MUTEX);

    if (_ENGINE != nullptr)
    {
        return _ENGINE->_currentWindow;
    }

    // Init GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    if (headlessMode)
    {
        glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
    }

    // Create engine instance and starting context
    _ENGINE = new Engine();
    glfwMakeContextCurrent(_ENGINE->_currentWindow._glfwWindow);

    // Init GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        throw std::runtime_error("Failed to initialize GLAD");
    }

    // Set viewport
    _ENGINE->_currentWindow.makeCurrent();

    // Load default shader programs
    ShaderManager::linkDefaults();

    return _ENGINE->_currentWindow;
}

void Engine::stop()
{
    std::lock_guard<std::mutex> lock(_MUTEX);

    if (_ENGINE == nullptr)
    {
        return;
    }

    // Terminate GLFW
    glfwTerminate();

    // Clear shader programs
    ShaderManager::clear();

    delete _ENGINE;
}

Window* Engine::getCurrentWindow()
{
    assertInitialized();
    return &_ENGINE->_currentWindow;
}

void Engine::setCurrentWindow(Window& window)
{
    _ENGINE->_currentWindow = window;
    window.makeCurrent();
}

Window Engine::createWindow()
{
    assertInitialized();
    return Window();
}

Buffer Engine::createBuffer()
{
    assertInitialized();
    return Buffer();
}

void Engine::renderFrame()
{
    assertInitialized();

    Window window = _ENGINE->_currentWindow;

    // Process input
    InputController* inputController = window.getInputController();
    inputController->processInput();

    // Clear
    Color clearColor = window._clearColor;
    glClearColor(clearColor.red, clearColor.green, clearColor.blue, clearColor.alpha);
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Render
    glfwSwapBuffers(window._glfwWindow);
    glfwPollEvents();
}

void Engine::assertInitialized()
{
    if (_ENGINE == nullptr)
    {
        throw IllegalStateException("Graphics engine has not been initialized");
    }
}