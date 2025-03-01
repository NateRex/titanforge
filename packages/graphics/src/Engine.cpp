#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <graphics/Engine.h>
#include <graphics/window/Window.h>
#include <graphics/Buffer.h>
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

    _INIT = true;
}

void Engine::stop()
{
    std::lock_guard<std::mutex> lock(_MUTEX);

    if (!_INIT)
    {
        return;
    }

    glfwTerminate();
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
    return Buffer();
}

void Engine::assertInitialized()
{
    if (!_INIT)
    {
        throw IllegalStateException("Graphics engine has not been initialized");
    }
}