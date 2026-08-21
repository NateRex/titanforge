#include <graphics/core/renderer/Renderer.h>
#include <graphics/core/renderer/RenderState.h>
#include <graphics/core/renderer/RenderTarget.h>
#include <graphics/core/renderer/RenderTarget.h>
#include <graphics/core/windows/Window.h>
#include <graphics/core/input/InputController.h>
#include <graphics/core/shaders/ShaderManager.h>
#include <graphics/core/shaders/Shader.h>
#include <graphics/core/buffers/GeometryBuffer.h>
#include <graphics/core/buffers/FrameBuffer.h>
#include <graphics/scene/Scene.h>
#include <graphics/cameras/Camera.h>
#include <graphics/lights/Light.h>
#include <graphics/materials/MeshMaterial.h>
#include <graphics/materials/PostProcessMaterial.h>
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
	
	if (!window || !window->_glfwWindow)
	{
		throw std::runtime_error("Renderer window must be an open window");
	}
	_window = window;

	// Set window as the current context
	glfwMakeContextCurrent(window->_glfwWindow);

	// Load GLAD function pointers
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
	glfwGetFramebufferSize(window->_glfwWindow, &width, &height);
	glViewport(0, 0, width, height);

	// Hide and capture the cursor inside the window
	glfwSetInputMode(_window->_glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	applyGlobalSettings();
}

Renderer::~Renderer()
{
	if (_fullScreenVertexArray != 0)
	{
		glDeleteVertexArrays(1, &_fullScreenVertexArray);
		_fullScreenVertexArray = 0;
	}
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
	if (_destroyed)
	{
		return;
	}

	_destroyed = true;
	decrementRendererCount();

	if (destroyWindow && _window)
	{
		_window->destroy();
	}
}

void Renderer::render(const ScenePtr scene, const CameraPtr camera, const PostProcessMaterialPtr& postProcessMaterial)
{
	if (_destroyed || !_window || !_window->_glfwWindow)
	{
		throw std::runtime_error("Cannot render with a destroyed renderer or window");
	}

	// If there's no post-processing material, we only need a single pass to the default framebuffer
	if (!postProcessMaterial)
	{
		RenderPass pass;
		pass.clearColor = _backgroundColor;
		renderPass(scene, camera, pass);
		present();
		return;
	}

	int framebufferWidth, framebufferHeight;
	glfwGetFramebufferSize(_window->_glfwWindow, &framebufferWidth, &framebufferHeight);
	if (framebufferWidth <= 0 || framebufferHeight <= 0)
	{
		throw std::runtime_error("Cannot post-process a window with an empty framebuffer");
	}

	// Set up / resize offscreen render target
	if (!_postProcessTarget)
	{
		RenderTargetDescriptor descriptor;
		descriptor.width = static_cast<unsigned int>(framebufferWidth);
		descriptor.height = static_cast<unsigned int>(framebufferHeight);
		descriptor.colorFormats = { PixelFormat::RGBA16F };
		_postProcessTarget = std::make_unique<RenderTarget>(descriptor);
	}
	else
	{
		_postProcessTarget->resize(
			static_cast<unsigned int>(framebufferWidth),
			static_cast<unsigned int>(framebufferHeight));
	}

	// Render the scene to the offscreen render target
	RenderPass scenePass;
	scenePass.target = _postProcessTarget.get();
	scenePass.clearColor = _backgroundColor;
	renderPass(scene, camera, scenePass);

	const TexturePtr originalTexture = postProcessMaterial->texture;
	postProcessMaterial->texture = _postProcessTarget->colorTexture(0);

	// Render post-processing effects
	try
	{
		RenderPass postProcessPass;
		postProcessPass.clearFlags = ClearFlags::COLOR;
		postProcessPass.depthTest = false;
		postProcessPass.depthWrite = false;
		postProcessPass.blending = false;
		postProcessPass.faceCulling = false;
		renderPass(postProcessMaterial, postProcessPass);
	}
	catch (...)
	{
		postProcessMaterial->texture = originalTexture;
		throw;
	}

	postProcessMaterial->texture = originalTexture;

	// Present the scene
	present();
}

void Renderer::renderPass(const ScenePtr scene, const CameraPtr camera, const RenderPass& pass)
{
	configurePass(pass);
	
	try
	{
		draw(traverseScene(scene, camera));
	}
	catch (...)
	{
		finishPass();
		throw;
	}
	finishPass();
}

void Renderer::renderPass(const PostProcessMaterialPtr& material, const RenderPass& pass)
{
	if (!material)
	{
		throw std::runtime_error("Post-process material cannot be null");
	}

	configurePass(pass);

	try
	{
		if (_fullScreenVertexArray == 0)
		{
			glGenVertexArrays(1, &_fullScreenVertexArray);
		}

		glDisable(GL_DEPTH_TEST);
		glDepthMask(GL_FALSE);
		glDisable(GL_CULL_FACE);

		ShaderPtr shader = ShaderManager::getShader(material->materialType);
		shader->activate();
		shader->setMaterial(material);
		
		glBindVertexArray(_fullScreenVertexArray);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
	catch (...)
	{
		finishPass();
		throw;
	}

	finishPass();
}

void Renderer::configurePass(const RenderPass& pass)
{
	// Bind framebuffer
	if (pass.target)
	{
		pass.target->frameBuffer()->bind();
	}
	else
	{
		FrameBuffer::bindDefault();
	}

	// Configure viewport
	unsigned int width = pass.viewport.width;
	unsigned int height = pass.viewport.height;
	if (width == 0 || height == 0)
	{
		if (pass.target)
		{
			width = pass.target->descriptor().width;
			height = pass.target->descriptor().height;
		}
		else
		{
			int w, h;
			glfwGetFramebufferSize(_window->_glfwWindow, &w, &h);
			width = static_cast<unsigned int>(w);
			height = static_cast<unsigned int>(h);
		}
	}
	glViewport(pass.viewport.x, pass.viewport.y, width, height);

	// Configure depth test, blending, and face culling
	pass.depthTest ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
	glDepthMask(pass.depthWrite ? GL_TRUE : GL_FALSE);
	pass.blending ? glEnable(GL_BLEND) : glDisable(GL_BLEND);
	pass.faceCulling ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);

	// Clear color, depth, and stencil buffers
	GLbitfield clearMask = 0;
	if (hasFlag(pass.clearFlags, ClearFlags::COLOR))
	{
		glClearColor(pass.clearColor.red(), pass.clearColor.green(), pass.clearColor.blue(), pass.clearColor.alpha());
		clearMask |= GL_COLOR_BUFFER_BIT;
	}
	if (hasFlag(pass.clearFlags, ClearFlags::DEPTH))
	{
		glClearDepth(pass.clearDepth);
		clearMask |= GL_DEPTH_BUFFER_BIT;
	}
	if (hasFlag(pass.clearFlags, ClearFlags::STENCIL))
	{
		glClearStencil(pass.clearStencil);
		clearMask |= GL_STENCIL_BUFFER_BIT;
	}
	if (clearMask != 0)
	{
		glClear(clearMask);
	}
}

void Renderer::finishPass()
{
	FrameBuffer::bindDefault();
	glDepthMask(GL_TRUE);
}

void Renderer::present()
{
	const float time = getTime();
	_window->getInputController()->pollForKeyHolds(time - _timeOfLastFrame);
	_timeOfLastFrame = time;
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
	const Vector3 cameraForward = state.camera->getForwardVector();
	std::stable_sort(blendedItems.begin(), blendedItems.end(), [&cameraPosition, &cameraForward](const RenderItem* a, const RenderItem* b)
	{
		const Vector3 aPosition = a->modelTransform.transformPosition(Vector3::ZERO);
		const Vector3 bPosition = b->modelTransform.transformPosition(Vector3::ZERO);
		const float aDepth = aPosition.minus(cameraPosition).dot(cameraForward);
		const float bDepth = bPosition.minus(cameraPosition).dot(cameraForward);
		return aDepth > bDepth;
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
	if (mesh->material->doubleSided)
	{
		glDisable(GL_CULL_FACE);
	}
	else
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}

	ShaderPtr shader = ShaderManager::getShader(mesh->material->materialType);
	shader->activate();
	shader->setState(state);
	shader->setItem(item);

	GeometryBuffer* buffer = mesh->geometry->getBuffer();
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

	// Disable stencil test by default
	glDisable(GL_STENCIL_TEST);
}
