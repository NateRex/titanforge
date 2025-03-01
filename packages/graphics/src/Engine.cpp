#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <graphics/Engine.h>
#include <graphics/window/Window.h>
#include <graphics/Buffer.h>
#include <graphics/shaders/ShaderManager.h>
#include <graphics/shaders/IShader.h>
#include <common/exceptions/IllegalStateException.h>

bool Engine::_INIT = false;
std::mutex Engine::_MUTEX;

Engine::Engine()
{

}

void Engine::start()
{
    std::lock_guard<std::mutex> lock(_MUTEX);

    if (_INIT)
    {
        return;
    }

    // Init GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    // Create starting window context
    _CURRENT_WINDOW = createWindow();
    _CURRENT_WINDOW.setContext();

    // Init GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        throw std::runtime_error("Failed to initialize GLAD");
    }

    // Load default shaders
    ShaderManager::registerDefaults();

    _INIT = true;
}

void Engine::stop()
{
    std::lock_guard<std::mutex> lock(_MUTEX);

    if (!_INIT)
    {
        return;
    }

    // Terminate GLFW
    glfwTerminate();

    // Unload shaders
    ShaderManager::clear();

    _INIT = false;
}

void Engine::setContext(Window& window)
{
    std::lock_guard<std::mutex> lock(_MUTEX);
    _CURRENT_WINDOW = window;
    _CURRENT_WINDOW.setContext();
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
    if (!_INIT)
    {
        throw IllegalStateException("Graphics engine has not been initialized");
    }
}