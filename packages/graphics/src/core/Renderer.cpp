#include <graphics/core/Renderer.h>
#include <graphics/core/windows/Window.h>
#include <common/Assertions.h>
#include <GLFW/glfw3.h>
#include <glad/glad.h>

Renderer::Renderer(): Renderer(Window::create("TitanForge", 800, 600))
{

}

Renderer::Renderer(WindowPtr window): _backgroundColor(Color::BLACK)
{
	setWindow(window);
}

Renderer::~Renderer()
{
	_window = nullptr;
	glfwTerminate();
}

WindowPtr Renderer::getWindow() const
{
	return _window;
}

void Renderer::setWindow(WindowPtr window)
{
	_window = window;

	// Set window as the current context
	glfwMakeContextCurrent(window->_glfwWindow);

	// Re-load GLAD function pointers
	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
	{
		throw std::runtime_error("Failed to initialize GLAD");
	}

	// Set the viewport dimensions
	int width, height;
	glfwGetWindowSize(window->_glfwWindow, &width, &height);
	glViewport(0, 0, width, height);
}

Color Renderer::getBackgroundColor() const
{
	return _backgroundColor;
}

void Renderer::setBackgroundColor(const Color& color)
{
	_backgroundColor = color;
}

void Renderer::initGLAD(const WindowPtr window)
{
	
}

void Renderer::applyGlobalDrawSettings()
{
	// Enable alpha channel for transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
}