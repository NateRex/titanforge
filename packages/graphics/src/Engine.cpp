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

Window Engine::createWindow()
{
    assertInitialized();
    return Window();
}

Buffer Engine::createBuffer()
{
    assertInitialized();
    assertWindowContext();
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

void Engine::assertWindowContext()
{
    if (glfwGetCurrentContext() == nullptr)
    {
        throw IllegalStateException("No window has been created or assigned to the context");
    }
}