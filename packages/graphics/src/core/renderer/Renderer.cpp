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
#include <graphics/materials/MeshMaterial.h>
#include <graphics/materials/PostProcessMaterial.h>
#include <graphics/loaders/TextureLoader.h>
#include <graphics/geometry/Geometry.h>
#include <graphics/geometry/GeometryAttributes.h>
#include <math/Matrix3.h>
#include <math/Matrix4.h>
#include <common/Utils.h>
#include <common/Assertions.h>
#include <common/exceptions/IllegalArgumentException.h>
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
		throw IllegalStateException("Renderer window must be an open window");
	}
	_window = window;

	// Set window as the current context
	glfwMakeContextCurrent(window->_glfwWindow);

	// Load GLAD function pointers
	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
	{
		throw IllegalStateException("Failed to initialize GLAD for window");
	}

	GLint value;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &value);

	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
	{
		std::ostringstream oss;
		oss << "Error occurred updating renderer window: " << err;
		throw IllegalStateException(oss.str());
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
		throw IllegalStateException("Cannot render with a destroyed renderer or window");
	}

	// If there's no post-processing material, we only need a single pass to the default framebuffer
	if (!postProcessMaterial)
	{
		RenderPass pass;
		pass.clearColor = _backgroundColor;
		renderPass(scene, camera, pass);
		return;
	}

	int framebufferWidth, framebufferHeight;
	glfwGetFramebufferSize(_window->_glfwWindow, &framebufferWidth, &framebufferHeight);
	if (framebufferWidth <= 0 || framebufferHeight <= 0)
	{
		throw IllegalStateException("Cannot post-process a window with an empty framebuffer");
	}

	// Set up the offscreen render target
	if (!_postProcessTarget)
	{
		RenderTargetConfig config;
		config.width = static_cast<unsigned int>(framebufferWidth);
		config.height = static_cast<unsigned int>(framebufferHeight);
		config.colorFormats = { PixelFormat::RGBA16F };
		_postProcessTarget = std::make_unique<RenderTarget>(config);
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

	// Render post-processing effects to the default framebuffer
	try
	{
		RenderPass postProcessPass;
		postProcessPass.clearFlags = ClearFlags::COLOR;
		renderPass(postProcessMaterial, postProcessPass);
	}
	catch (...)
	{
		postProcessMaterial->texture = originalTexture;
		throw;
	}

	postProcessMaterial->texture = originalTexture;
}

void Renderer::renderPass(const ScenePtr scene, const CameraPtr camera, const RenderPass& pass)
{
	if (!pass.mode)
	{
		throw IllegalArgumentException("Render pass mode cannot be null");
	}

	configurePass(pass);
	
	try
	{
		// Materials control depth writes, blending, and culling per item.
		// Depth testing itself applies to the entire scene pass.
		glEnable(GL_DEPTH_TEST);

		RenderState state;
		scene->traverse(state, Matrix4::IDENTITY, Matrix3::IDENTITY);
		draw(state, *pass.mode, camera);
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
		throw IllegalArgumentException("Post-process material cannot be null");
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
		glDisable(GL_BLEND);
		glDisable(GL_CULL_FACE);

		ShaderPtr shader = ShaderManager::getShader(ShaderId::POST_PROCESS);
		shader->activate();
		shader->setMaterial(material.get());
		
		// Post-processing shader operates on raw vertex indices, rather than actual vertex data. Therefore, we do not actually
		// need to pass vertex values as part of the buffer.
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
			width = pass.target->config().width;
			height = pass.target->config().height;
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
	if (_destroyed || !_window || !_window->_glfwWindow)
	{
		throw IllegalStateException("Cannot present with a destroyed renderer or window");
	}

	const float time = getTime();
	_window->getInputController()->pollForKeyHolds(time - _timeOfLastFrame);
	_timeOfLastFrame = time;
	glfwSwapBuffers(_window->_glfwWindow);
	glfwPollEvents();
}

void Renderer::draw(RenderState& state, const RenderMode& mode, const CameraPtr camera)
{
	// Group opaque, background, and transparent items
	std::vector<RenderItem*> opaqueItems;
	std::vector<RenderItem*> backgroundItems;
	std::vector<RenderItem*> transparentItems;
	for (RenderItem& item : state.items)
	{
		mode.apply(item);
		if (!item.visible)
		{
			continue;
		}

		switch (item.layer)
		{
			case RenderLayer::TRANSPARENT: transparentItems.push_back(&item); break;
			case RenderLayer::BACKGROUND: backgroundItems.push_back(&item); break;
			case RenderLayer::OPAQUE: opaqueItems.push_back(&item); break;
			default: opaqueItems.push_back(&item); break;
		}
	}

	// Sort transparent items from farthest to closest, relative to the camera
	const Vector3 cameraPosition = camera->getPosition();
	const Vector3 cameraForward = camera->getForwardVector();
	std::stable_sort(transparentItems.begin(), transparentItems.end(), [&cameraPosition, &cameraForward](const RenderItem* a, const RenderItem* b)
	{
		const Vector3 aPosition = a->modelTransform.transformPosition(Vector3::ZERO);
		const Vector3 bPosition = b->modelTransform.transformPosition(Vector3::ZERO);
		const float aDepth = aPosition.minus(cameraPosition).dot(cameraForward);
		const float bDepth = bPosition.minus(cameraPosition).dot(cameraForward);
		return aDepth > bDepth;
	});

	// Render opaque items
	for (const RenderItem* item : opaqueItems)
	{
		drawItem(state, *item, camera);
	}

	// Render background scenery
	for (const RenderItem* item : backgroundItems)
	{
		drawItem(state, *item, camera);
	}

	// Render transparent items
	for (const RenderItem* item : transparentItems)
	{
		drawItem(state, *item, camera);
	}
}

void Renderer::drawItem(const RenderState& state, const RenderItem& item, const CameraPtr camera)
{
	Geometry* geometry = item.geometry;
	GeometryAttributes geometryAttrib = geometry->getAttributes();
	Material* material = item.material;
	const bool transparent = item.layer == RenderLayer::TRANSPARENT;

	// Set global state
	glDepthMask(material->depthWrite && !transparent ? GL_TRUE : GL_FALSE);
	switch (material->depthFunction)
	{
		case DepthFunction::ALWAYS: glDepthFunc(GL_ALWAYS); break;
		case DepthFunction::NEVER: glDepthFunc(GL_NEVER); break;
		case DepthFunction::LESS: glDepthFunc(GL_LESS); break;
		case DepthFunction::LESS_OR_EQUAL_TO: glDepthFunc(GL_LEQUAL); break;
		case DepthFunction::GREATER: glDepthFunc(GL_GREATER); break;
		case DepthFunction::GREATER_OR_EQUAL_TO: glDepthFunc(GL_GEQUAL); break;
		case DepthFunction::NOT_EQUAL: glDepthFunc(GL_NOTEQUAL); break;
		case DepthFunction::EQUAL: glDepthFunc(GL_EQUAL); break;
		default: glDepthFunc(GL_LESS); break;
	}
	switch (material->cullingMode)
	{
		case CullingMode::NONE: glDisable(GL_CULL_FACE); break;
		case CullingMode::FRONT: glEnable(GL_CULL_FACE); glCullFace(GL_FRONT); break;
		case CullingMode::BACK: glEnable(GL_CULL_FACE); glCullFace(GL_BACK); break;
		default: glDisable(GL_CULL_FACE); break;
	}
	if (transparent)
	{
		glEnable(GL_BLEND);
	}
	else {
		glDisable(GL_BLEND);
	}

	// Activate shader
	ShaderPtr shader = ShaderManager::getShader(item.shader);
	shader->activate();
	shader->setState(state);
	shader->setItem(item);
	shader->setCamera(camera.get());

	// Draw buffer
	GeometryBuffer* buffer = geometry->getBuffer();
	unsigned int primitiveType = toGLPrimitive(geometry->type);
	buffer->bind();
	if (geometryAttrib.indices)
	{
		glDrawElements(primitiveType, buffer->size(), GL_UNSIGNED_INT, 0);
	}
	else {
		glDrawArrays(primitiveType, 0, buffer->size());
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
	// Allow linear filtering across cubemap face boundaries instead of clamping
	// each face independently, which otherwise exposes visible skybox seams.
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	// Enable setting point size in shaders for point primitive draws
	glEnable(GL_PROGRAM_POINT_SIZE);

	// Configure alpha blending used by transparent materials
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Disable stencil test by default
	glDisable(GL_STENCIL_TEST);
}
