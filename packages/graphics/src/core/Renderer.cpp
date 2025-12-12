#include <graphics/core/Renderer.h>
#include <graphics/core/RenderState.h>
#include <graphics/core/windows/Window.h>
#include <graphics/core/input/InputController.h>
#include <graphics/core/shaders/ShaderManager.h>
#include <graphics/core/shaders/Shader.h>
#include <graphics/core/Buffer.h>
#include <graphics/scene/Scene.h>
#include <graphics/cameras/Camera.h>
#include <graphics/materials/Material.h>
#include <graphics/textures/TextureLoader.h>
#include <graphics/geometry/Geometry.h>
#include <common/Utils.h>
#include <common/Assertions.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <sstream>

int Renderer::_RENDERER_COUNT = 0;
std::mutex Renderer::_MUTEX;

Renderer::Renderer(WindowPtr window): _backgroundColor(Color::BLACK)
{
	incrementRendererCount();
	setWindow(window);
	applyGlobalSettings();
}

Renderer::~Renderer()
{
	destroy();
}

RendererPtr Renderer::create(WindowPtr window)
{
	WindowPtr target = window ? window : Window::create("TitanForge", 800, 600);
	return std::shared_ptr<Renderer>(new Renderer(target));
}

float Renderer::getTime() const
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

	// Hide and capture the cursor inside the window
	glfwSetInputMode(_window->_glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

Color Renderer::getBackgroundColor() const
{
	return _backgroundColor;
}

void Renderer::setBackgroundColor(const Color& color)
{
	_backgroundColor = color;
}

void Renderer::destroy(bool destroyWindow)
{
	decrementRendererCount();

	if (destroyWindow && _window)
	{
		_window->destroy();
	}
}

void Renderer::render(const ScenePtr scene, const CameraPtr camera)
{
	// Clear
	glClearColor(_backgroundColor.red(), _backgroundColor.green(), _backgroundColor.blue(), _backgroundColor.alpha());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Poll for input updates
	float time = getTime();
	float deltaTime = time - _timeOfLastFrame;
	_timeOfLastFrame = time;
	_window->getInputController()->pollForKeyHolds(deltaTime);

	// Traverse scene
	RenderState state;
	state.camera = camera;
	traverseScene(scene, Matrix4::IDENTITY, state);

	// Render state
	draw(state);

	// Display scene
	glfwSwapBuffers(_window->_glfwWindow);
	glfwPollEvents();
}

void Renderer::traverseScene(const EntityPtr entity, const Matrix4& parentTransform, RenderState& state)
{
	Matrix4 transform = parentTransform.multiply(entity->getMatrix());

	switch (entity->type)
	{
		case EntityType::GROUP:
		{
			// Recursively handle each child
			for (const EntityPtr child : entity->_children)
			{
				traverseScene(child, transform, state);
			}
			break;
		}
		case EntityType::MESH:
		{
			RenderItem renderItem;
			renderItem.mesh = cast<Mesh>(entity);
			renderItem.local2World = transform;
			
			state.items.push_back(renderItem);
		}
		default:
		{
			// Do nothing
		}
	}
}

void Renderer::draw(const RenderState& state)
{
	CameraPtr camera = state.camera;

	for (const RenderItem& item : state.items)
	{
		MeshPtr mesh = item.mesh;
		const Matrix4& local2World = item.local2World;

		// Load shader data
		MaterialPtr material = mesh->material;
		ShaderPtr shader = ShaderManager::getShader(material->type);
		shader->activate();
		shader->setModelMatrix(local2World);
		shader->setViewMatrix(camera->getViewMatrix());
		shader->setProjectionMatrix(camera->getProjectionMatrix());
		shader->setMaterial(material);

		// Draw buffer
		Buffer* buffer = mesh->geometry->getBuffer();
		buffer->bind();
		glDrawElements(GL_TRIANGLES, buffer->size, GL_UNSIGNED_INT, 0);
	}
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

void Renderer::applyGlobalSettings()
{
	// Enable alpha channel for transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
}