#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <graphics/Engine.h>
#include <graphics/windows/Window.h>
#include <graphics/windows/InputController.h>
#include <graphics/windows/WindowManager.h>
#include <graphics/shaders/ShaderManager.h>
#include <graphics/textures/TextureManager.h>
#include <common/exceptions/IllegalStateException.h>

std::mutex Engine::_MUTEX;
bool Engine::_INITIALIZED = false;

void Engine::start(bool headlessMode)
{
    std::lock_guard<std::mutex> lock(_MUTEX);

    if (_INITIALIZED)
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

    if (headlessMode)
    {
        glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
    }

    // Initialize starting window
    WindowManager::setup();
    Window* window = WindowManager::create("tf_default_window");
    glfwMakeContextCurrent(window->_glfwWindow);

    // Init GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        throw std::runtime_error("Failed to initialize GLAD");
    }

    applyGlobalGLSettings();

    // Set current window
    WindowManager::setCurrent("tf_default_window");

    // Initialize shaders
    ShaderManager::setup();

    // Initialize textures
    TextureManager::setup();

    _INITIALIZED = true;
}

void Engine::stop()
{
    std::lock_guard<std::mutex> lock(_MUTEX);

    if (!_INITIALIZED)
    {
        return;
    }

    // Clear managers
    TextureManager::clear();
    ShaderManager::clear();
    WindowManager::clear();

    // Terminate GLFW
    glfwTerminate();

    _INITIALIZED = false;
}

double Engine::getTime()
{
    assertInitialized();
    return glfwGetTime();
}

void Engine::startFrame()
{
    assertInitialized();

    Window* window = WindowManager::getCurrent();

    // Process input
    InputController* inputController = window->getInputController();
    inputController->processInput();

    // Clear
    Color clearColor = window->_clearColor;
    glClearColor(clearColor.red(), clearColor.green(), clearColor.blue(), clearColor.alpha());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Engine::finishFrame()
{
    assertInitialized();

    // Render
    Window* window = WindowManager::getCurrent();
    glfwSwapBuffers(window->_glfwWindow);
    glfwPollEvents();
}

void Engine::assertInitialized()
{
    if (!_INITIALIZED)
    {
        throw IllegalStateException("Graphics engine has not been initialized");
    }
}

void Engine::applyGlobalGLSettings()
{
    // Enable alpha channel for transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
}