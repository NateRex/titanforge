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
#include <graphics/textures/pointers/TexturePtr.h>

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
	void getWindowDimensions(float* width, float* height) const;

	/**
	 * @return The current linear RGB background clear color for draw commands
	 */
	Color getBackgroundColor() const;

	/**
	 * Sets the current linear RGB background clear color for draw commands
	 * @param color Linear RGB background color with unencoded alpha
	 */
	void setBackgroundColor(const Color& color);

    /**
	 * @return Display adjustment exponent; neutral default is 1.
	 */
    float getDisplayGamma() const;

    /**
     * Sets display adjustment, applied after all linear postprocessing and before fixed sRGB encoding.
     * RGB is clamped to [0,1] before pow(rgb, 1 / displayGamma); no tone mapping is performed.
     * Alpha is unchanged by display adjustment and encoding.
     * @param displayGamma Finite positive exponent; defaults to 1.
     * @throws IllegalArgumentException If the value is nonpositive or nonfinite; retains the previous value.
     */
    void setDisplayGamma(float displayGamma);

	/**
	 * Renders lighting, blending, and effects in linear RGB, then encodes sRGB exactly once and presents.
	 * Custom shaders must consume and produce linear RGB; alpha is never gamma encoded.
	 * Existing scenes tuned for unencoded output need color and lighting retuning.
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
	 * Number of samples per pixel used for multisample anti-aliasing. Regardless of whether or not the window was created
	 * with anti-aliasing enabled, we determine and cache this value in the renderer since the system may not be able to
	 * handle the number of samples requested at window creation time.
	 */
	unsigned int _samples = 1;

	/**
	 * Boolean flag that, when true, means that this renderer has released its resources and global renderer count.
	 */
	bool _destroyed = false;

	/**
	 * The current background (clear) color
	 */
	Color _backgroundColor;

	/**
	 * Display adjustment exponent
	 */
	float _displayGamma = 1.f;

	/**
	 * Renderer-managed intermediate targets used to chain scene post-processing effects
	 */
	RenderTargetPtr _postProcessTargets[2];

	/**
	 * Multisampled scene target resolved before post-processing. Will only be non-null if sampling is greater than 1,
	 * which occurs when anti-aliasing is enabled.
	 */
	RenderTargetPtr _postProcessMultiSampleTarget;

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
	void getPassDimensions(const RenderPass& pass, float* width, float* height) const;

	/**
	 * Configures and draws final display conversion, swaps buffers, and processes window events.
	 * @param source Linear scene texture after all effects.
	 */
	void present(const TexturePtr& source);

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
