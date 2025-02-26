#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <graphics/Engine.h>
#include <stdexcept>
#include <cstdlib>

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