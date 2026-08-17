#include <graphics/core/Renderer.h>
#include <graphics/core/RenderState.h>
#include <graphics/core/windows/Window.h>
#include <graphics/core/input/InputController.h>
#include <graphics/core/shaders/ShaderManager.h>
#include <graphics/core/shaders/Shader.h>
#include <graphics/core/Buffer.h>
#include <graphics/scene/Scene.h>
#include <graphics/cameras/Camera.h>
#include <graphics/lights/Light.h>
#include <graphics/materials/Material.h>
#include <graphics/loaders/TextureLoader.h>
#include <graphics/geometry/Geometry.h>
#include <math/Matrix3.h>
#include <math/Matrix4.h>
#include <common/Utils.h>
#include <common/Assertions.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <sstream>
#include <algorithm>

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

float Renderer::getDeltaTime() const
{
	return getTime() - _timeOfLastFrame;
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

	// Traverse scene and draw to buffer
	RenderState state = traverseScene(scene, camera);
	draw(state);

	// Swap buffers to display scene
	glfwSwapBuffers(_window->_glfwWindow);
	glfwPollEvents();
}

RenderState Renderer::traverseScene(const ScenePtr scene, const CameraPtr camera)
{
	RenderState state;
	state.camera = camera;
	traverseScene(scene, Matrix4::IDENTITY, Matrix3::IDENTITY, state);
	return state;
}

void Renderer::traverseScene(const EntityPtr entity, const Matrix4& parentModel, const Matrix3& parentNormal, RenderState& state)
{
	const Matrix4 modelTransform = parentModel.multiply(entity->getLocalMatrix());
	const Matrix3 normalTransform = parentNormal.multiply(entity->getLocalNormalMatrix());

	switch (entity->entityType)
	{
		case EntityType::LIGHT:
		{
			const LightPtr light = cast<Light>(entity);
			
			RenderLight renderLight;
			renderLight.light = light;
			renderLight.position = parentModel.transformPosition(light->getPosition());
			renderLight.direction = parentModel.transformDirection(light->getForwardVector()).normalize();
			
			state.lighting.lights.push_back(renderLight);
			break;
		}
		case EntityType::MESH:
		{
			RenderItem renderItem;
			renderItem.mesh = cast<Mesh>(entity);
			renderItem.modelTransform = modelTransform;
			renderItem.normalTransform = normalTransform;

			state.items.push_back(renderItem);
			break;
		}
		default:
		{
			// Do nothing
			break;
		}
	}

	// Entity type does not affect hierarchy. Any type of entity may have children.
	for (const EntityPtr child : entity->_children)
	{
		traverseScene(child, modelTransform, normalTransform, state);
	}
}

void Renderer::draw(const RenderState& state)
{
	// Sort opaque (solid) and transparent (blended) items
	std::vector<const RenderItem*> solidItems;
	std::vector<const RenderItem*> blendedItems;
	for (const RenderItem& item : state.items)
	{
		if (item.mesh->material->getEffectiveAlphaMode() == AlphaMode::BLEND)
		{
			blendedItems.push_back(&item);
		}
		else {
			solidItems.push_back(&item);
		}
	}

	const Vector3 cameraPosition = state.camera->getPosition();
	std::stable_sort(blendedItems.begin(), blendedItems.end(), [&cameraPosition](const RenderItem* a, const RenderItem* b)
	{
		const Vector3 aPosition = a->modelTransform.transformPosition(Vector3::ZERO);
		const Vector3 bPosition = b->modelTransform.transformPosition(Vector3::ZERO);
		const Vector3 aOffset = aPosition.minus(cameraPosition);
		const Vector3 bOffset = bPosition.minus(cameraPosition);
		return aOffset.dot(aOffset) > bOffset.dot(bOffset);
	});

	// Opaque pass
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
	for (const RenderItem* item : solidItems)
	{
		drawItem(state, *item);
	}

	// Transparent pass
	glEnable(GL_BLEND);
	glDepthMask(GL_FALSE);
	for (const RenderItem* item : blendedItems)
	{
		drawItem(state, *item);
	}

	glDepthMask(GL_TRUE);
}

void Renderer::drawItem(const RenderState& state, const RenderItem& item)
{
	MeshPtr mesh = item.mesh;
	ShaderPtr shader = ShaderManager::getShader(mesh->material->materialType);
	shader->activate();
	shader->setState(state);
	shader->setItem(item);

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

void Renderer::applyGlobalSettings()
{
	// Enable alpha channel for transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
}
