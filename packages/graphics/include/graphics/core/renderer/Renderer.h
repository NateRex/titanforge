#pragma once
#include <graphics/core/pointers/RendererPtr.h>
#include <graphics/core/windows/pointers/WindowPtr.h>
#include <graphics/core/windows/WindowFlags.h>
#include <graphics/scene/pointers/ScenePtr.h>
#include <graphics/cameras/pointers/CameraPtr.h>
#include <graphics/objects/Mesh.h>
#include <graphics/core/Color.h>
#include <graphics/core/renderer/RenderPass.h>
#include <mutex>

class Matrix3;
class Matrix4;
class RenderTarget;
class PostProcessMaterial;
struct DrawState;
struct DrawItem;

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
	 * @param windowFlags Settings to apply when creating a new window. These settings are only used if a previously-existing
	 * window was not provided. Defaults to WindowFlags::RESIZABLE.
	 * @return A pointer to the new renderer instance
	 */
	static RendererPtr create(WindowPtr window = nullptr, WindowFlags windowFlags = WindowFlags::RESIZABLE);

	/**
	 * @return The total amount of time (in decimal seconds) that this renderer has been active for
	 */
	float getTime() const;

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
	 * Renders and presents a scene.
	 * @param scene Scene
	 * @param camera Camera
	 * @param modes Bit mask containing all of the visualization modes to render. Defaults to RenderModes::MATERIAL.
	 */
	void render(const ScenePtr scene, const CameraPtr camera, RenderModes modes = RenderModes::MATERIAL);

	/**
	 * Destroys this renderer, releasing all of its resources
	 */
	void destroy();

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
	 * OpenGL ID for the lazily created render-managed vertex array object used for full-screen post-processing draws
	 */
	unsigned int _fullScreenVertexArray = 0;

	/**
	 * Renderer-managed intermediate targets used to chain scene post-processing effects
	 */
	RenderTargetPtr _postProcessTargets[2];

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
	 * Resolves the pixel dimensions of a render pass destination.
	 * @param pass Render pass settings
	 * @param width Value in which to store the destination width
	 * @param height Value in which to store the destination height
	 */
	void getPassDimensions(const RenderPass& pass, int* width, int* height) const;

	/**
	 * Presents the completed default frame buffer and processes input and window events. This method should be called once
	 * after composing all passes for a frame.
	 */
	void present();

	/**
	 * Configures the target, viewport, and clear operations for one render pass
	 * @param pass Render pass settings
	 */
	void configurePass(const RenderPass& pass);

	/**
	 * Consumes a prepared render state and submits draw calls for all items
	 * @param state The render state to draw
	 * @param mode Rendering mode
	 * @param camera Camera
	 */
	void draw(DrawState& state, RenderModes mode, const CameraPtr camera);

	/**
	 * Draws a single prepared render item
	 * @param state The render state
	 * @param item The item to draw
	 * @param mode Rendering mode
	 * @param camera Camera
	 */
	void drawItem(const DrawState& state, const DrawItem& item, RenderModes mode, const CameraPtr camera);

	/**
	 * Draws a single full-screen post-processing effect.
	 * @param material Post-processing material
	 */
	void drawPostProcessing(PostProcessMaterial* material);
};
