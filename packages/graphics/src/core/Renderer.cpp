#include <graphics/core/Renderer.h>
#include <graphics/core/windows/Window.h>
#include <graphics/core/windows/InputController.h>
#include <graphics/core/shaders/ShaderManager.h>
#include <graphics/core/shaders/Shader.h>
#include <graphics/core/Buffer.h>
#include <graphics/scene/Scene.h>
#include <graphics/materials/Material.h>
#include <graphics/textures/TextureLoader.h>
#include <graphics/geometry/Geometry.h>
#include <math/Matrix4.h>		// TODO: Delete this include when view and proj matrices are coming from camera
#include <common/Utils.h>		// TODO: Delete this include when view and proj matrices are coming from camera
#include <common/Assertions.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <sstream>

int Renderer::_RENDERER_COUNT = 0;
std::mutex Renderer::_MUTEX;

Renderer::Renderer(): Renderer(Window::create("TitanForge", 800, 600))
{

}

Renderer::Renderer(WindowPtr window): _backgroundColor(Color::BLACK)
{
	incrementRendererCount();
	setWindow(window);
	applyGlobalDrawSettings();
}

Renderer::~Renderer()
{
	decrementRendererCount();
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
	if (_window == window)
	{
		return;
	}

	_window = window;

	// Set window as the current context
	glfwMakeContextCurrent(window->_glfwWindow);

	// Re-load GLAD function pointers
	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
	{
		throw std::runtime_error("Failed to initialize GLAD for window");
	}

	GLint value;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &value);

	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
	{
		std::ostringstream oss;
		oss << "Error occurred updating renderer window: " << err;
		throw std::runtime_error(oss.str());
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

void Renderer::render(const ScenePtr scene) const
{
	// Process input
	InputController* inputController = _window->getInputController();
	inputController->processInput();

	// Clear
	glClearColor(_backgroundColor.red(), _backgroundColor.green(), _backgroundColor.blue(), _backgroundColor.alpha());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Recursively parse and draw entities
	renderEntity(scene, scene->getMatrix());

	// Display scene
	glfwSwapBuffers(_window->_glfwWindow);
	glfwPollEvents();
}

void Renderer::renderEntity(const EntityPtr entity, const Matrix4& local2World) const
{
	switch (entity->type)
	{
		case EntityType::GROUP:
		{
			// Recursively handle each child
			for (const EntityPtr child : entity->_children)
			{
				Matrix4 childMatrix = local2World.multiply(child->getMatrix());
				renderEntity(child, childMatrix);
			}
			break;
		}
		case EntityType::MESH:
		{
			// Handle mesh
			MeshPtr mesh = cast<Mesh>(entity);
			renderMesh(mesh, local2World);
			break;
		}
		default:
		{
			// Do nothing
		}
	}
}

void Renderer::renderMesh(const MeshPtr mesh, const Matrix4& local2World) const
{
	// Load shader data
	MaterialPtr material = mesh->material;
	ShaderPtr shader = ShaderManager::getShader(material->type);
	shader->use();
	shader->setModelMatrix(local2World);
	shader->setViewMatrix(Matrix4::fromTranslation(Vector3(0.f, 0.f, -3.f)));							// TODO: Get this data from camera
	shader->setProjectionMatrix(Matrix4::fromPerspective(deg2Rad(45.f), 800.f / 600.f, 0.1f, 100.f));	// TODO: Get this data from camera
	shader->setMaterial(material);

	// Draw buffer
	Buffer* buffer = mesh->geometry->getBuffer();
	buffer->bind();
	glDrawElements(GL_TRIANGLES, buffer->size, GL_UNSIGNED_INT, 0);
}

void Renderer::incrementRendererCount()
{
	std::lock_guard<std::mutex> lock(_MUTEX);
	_RENDERER_COUNT++;
}

void Renderer::decrementRendererCount()
{
	std::lock_guard<std::mutex> lock(_MUTEX);
	if (--_RENDERER_COUNT == 0)
	{
		TextureLoader::reset();
		ShaderManager::reset();
	}
}

void Renderer::applyGlobalDrawSettings()
{
	// Enable alpha channel for transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
}