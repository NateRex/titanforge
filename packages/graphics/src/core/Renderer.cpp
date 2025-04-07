#include <graphics/core/Renderer.h>
#include <graphics/core/windows/Window.h>
#include <graphics/core/windows/InputController.h>
#include <graphics/core/shaders/ShaderManager.h>
#include <graphics/core/shaders/Shader.h>
#include <graphics/core/Buffer.h>
#include <graphics/entities/Mesh.h>
#include <graphics/materials/Material.h>
#include <graphics/geometry/Geometry.h>
#include <math/Matrix4.h>		// TODO: Delete this include when view and proj matrices are coming from camera
#include <common/Utils.h>		// TODO: Delete this include when view and proj matrices are coming from camera
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
}

double Renderer::getTime() const
{
	return glfwGetTime();
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
		throw std::runtime_error("Failed to initialize GLAD for window");
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

void Renderer::render(const MeshPtr entity) const
{
	// Process input
	InputController* inputController = _window->getInputController();
	inputController->processInput();

	// Clear
	glClearColor(_backgroundColor.red(), _backgroundColor.green(), _backgroundColor.blue(), _backgroundColor.alpha());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Load shader data
	MaterialPtr material = entity->material;
	ShaderPtr shader = ShaderManager::getInstance().getShader(material->type);
	shader->use();
	shader->setModelMatrix(entity->getMatrix());
	shader->setViewMatrix(Matrix4::fromTranslation(Vector3(0.f, 0.f, -3.f)));							// TODO: Get this data from camera
	shader->setProjectionMatrix(Matrix4::fromPerspective(deg2Rad(45.f), 800.f / 600.f, 0.1f, 100.f));	// TODO: Get this data from camera
	shader->setMaterial(material);

	// Draw buffer
	Buffer* buffer = entity->geometry->getBuffer();
	buffer->bind();
	glDrawElements(GL_TRIANGLES, buffer->size, GL_UNSIGNED_INT, 0);

	// Display scene
	glfwSwapBuffers(_window->_glfwWindow);
	glfwPollEvents();
}

void Renderer::applyGlobalDrawSettings()
{
	// Enable alpha channel for transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
}