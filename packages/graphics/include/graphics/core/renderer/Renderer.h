#pragma once
#include <graphics/core/pointers/RendererPtr.h>
#include <graphics/core/windows/pointers/WindowPtr.h>
#include <graphics/scene/pointers/ScenePtr.h>
#include <graphics/cameras/pointers/CameraPtr.h>
#include <graphics/objects/Mesh.h>
#include <graphics/core/Color.h>
#include <graphics/core/renderer/RenderPass.h>
#include <graphics/materials/pointers/PostProcessMaterialPtr.h>
#include <mutex>

class Matrix3;
class Matrix4;
class RenderTarget;
struct RenderState;
struct RenderItem;

/**
 * The renderer is responsible for managing the current render context target and drawing the scene
 * @author Nathaniel Rex
 */
class Renderer
{
public:

	/**
	 * Destructor
	 */
	~Renderer();

	/**
	 * Constructs a new renderer instance
	 * @param window Existing window to attach the renderer to. Can be null, in which case a new window will automatically
	 * be created.
	 * @return A pointer to the new renderer instance.
	 */
	static RendererPtr create(WindowPtr window = nullptr);

	/**
	 * @return The total amount of time (in decimal seconds) that this renderer has been active for
	 */
	float getTime() const;

	/**
	 * @return The time (in decimal seconds) since the previous frame was rendered
	 */
	float getDeltaTime() const;

	/**
	 * @return The current window context that is the target of draw commands
	 */
	WindowPtr getWindow() const;

	/**
	 * Get the pixel dimensions of the window's default framebuffer
	 * @param width Value in which to store the width, in pixels
	 * @param height Value in which to store the height, in pixels
	 */
	void getWindowDimensions(int* width, int* height) const;

	/**
	 * @return The current background clear color for draw commands
	 */
	Color getBackgroundColor() const;

	/**
	 * Sets the current background clear color for draw commands
	 * @param color Background color
	 */
	void setBackgroundColor(const Color& color);

	/**
	 * Renders a scene, optionally applying a post-process material. This method does not present the completed frame;
	 * call Renderer::present after all render and renderPass calls belonging to the frame are complete.
	 * @param scene Scene
	 * @param camera Camera
	 * @param postProcessMaterial Optional material used to apply post-processing effects. When supplied, the scene is first
	 * rendered to an intermediate texture and the effect is applied before presentation.
	 */
	void render(
		const ScenePtr scene,
		const CameraPtr camera,
		const PostProcessMaterialPtr& postProcessMaterial = nullptr);

	/**
	 * Renders a scene once using an explicit pass configuration. This method does not present the completed frame;
	 * call Renderer::present after all render and renderPass calls belonging to the frame are complete.
	 * @param scene Scene graph to render.
	 * @param camera Camera used to view the scene.
	 * @param pass Visualization, render target, viewport, and clear operations for this draw.
	 */
	void renderPass(const ScenePtr scene, const CameraPtr camera, const RenderPass& pass);

	/**
	 * Applies a post-process material once using an explicit pass configuration. This method does not present the completed frame;
	 * call Renderer::present after all render and renderPass calls belonging to the frame are complete.
	 * @param material Post-process shader material to execute.
	 * @param pass Visualization, render target, viewport, and clear operations for this draw.
	 */
	void renderPass(const PostProcessMaterialPtr& material, const RenderPass& pass);

	/**
	 * Presents the completed default frame buffer and processes input and window events. Call this once after composing all
	 * passes for a frame.
	 */
	void present();

	/**
	 * Destroys this renderer, releasing all of its resources
	 * @param destroyWindow Boolean flag that, when true, causes the destruction of the window this renderer is
	 * currently attached to. Defaults to false.
	 */
	void destroy(bool destroyWindow = false);

private:

	/**
	 * The number of renderers currently in existence. This counter is used to determine when we need
	 * to release global resources, such as shaders or textures
	 */
	static int _RENDERER_COUNT;

	/**
	 * Mutex for modifying the global renderer count
	 */
	static std::mutex _MUTEX;

	/**
	 * The current window context
	 */
	WindowPtr _window;

	/**
	 * Boolean flag that, when true, means that this renderer has released its resources and global renderer count.
	 */
	bool _destroyed = false;

	/**
	 * The current background (clear) color
	 */
	Color _backgroundColor;

	/**
	 * Time (in decimal seconds) at which the last frame was rendered (relative to the start of rendering)
	 */
	float _timeOfLastFrame = 0.f;
	
	/**
	 * OpenGL ID of the lazily created vertex array object used for full-screen post-processing draws
	 */
	unsigned int _fullScreenVertexArray = 0;

	/**
	 * Renderer-managed intermediate target used to produce post-processing effects in cases where no render
	 * target is supplied by the user
	 */
	RenderTargetPtr _postProcessTarget;

	/**
	 * Increments the global renderer count
	 */
	static void incrementRendererCount();

	/**
	 * Decrements the global renderer count. If this is the last renderer to be destroyed, this will result
	 * in the release of global resources, such as textures and shaders.
	 */
	static void decrementRendererCount();

	/**
	 * Applies fixed global OpenGL settings that should be set for the entire system. This method only needs to
	 * be called once, on construction of the first renderer instance.
	 */
	static void applyGlobalSettings();

	/**
	 * Constructs a new renderer instance
	 * @param window Starting window target
	 */
	Renderer(WindowPtr window);

	/**
	 * Configures the target, viewport, and clear operations for one render pass
	 * @param pass Render pass settings
	 */
	void configurePass(const RenderPass& pass);

	/**
	 * Completes a render pass and restores the default frame buffer.
	 */
	void finishPass();

	/**
	 * Consumes a prepared render state and submits draw calls for all items
	 * @param state The render state to draw
	 * @param mode Visualization strategy for this pass
	 * @param camera Camera
	 */
	void draw(RenderState& state, const RenderMode& mode, const CameraPtr camera);

	/**
	 * Draws a single prepared render item
	 * @param state The render state
	 * @param item The item to draw
	 * @param camera Camera
	 */
	void drawItem(const RenderState& state, const RenderItem& item, const CameraPtr camera);
};
