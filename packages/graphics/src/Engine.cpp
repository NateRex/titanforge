#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <graphics/Engine.h>
#include <graphics/window/Window.h>
#include <graphics/Buffer.h>
#include <graphics/shaders/ShaderManager.h>
#include <graphics/shaders/IShader.h>
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

    // Load default shaders
    ShaderManager::registerDefaults();

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

    // Unload shaders
    ShaderManager::clear();

    delete _ENGINE;
}

Window Engine::getCurrentContext()
{
    assertInitialized();
    return _ENGINE->_currentWindow;
}

void Engine::setContext(Window& window)
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

const IShader* Engine::getShader(const std::string& name)
{
    assertInitialized();
    return ShaderManager::get(name);
}

void Engine::assertInitialized()
{
    if (_ENGINE == nullptr)
    {
        throw IllegalStateException("Graphics engine has not been initialized");
    }
}