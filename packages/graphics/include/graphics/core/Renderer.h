#pragma once
#include <graphics/core/pointers/RendererPtr.h>
#include <graphics/core/windows/pointers/WindowPtr.h>
#include <graphics/scene/pointers/ScenePtr.h>
#include <graphics/cameras/pointers/CameraPtr.h>
#include <graphics/objects/Mesh.h>
#include <graphics/core/Color.h>
#include <mutex>

struct RenderState;

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
	 * Sets the current window context that will be the target of draw commands
	 * @param window Window target
	 */
	void setWindow(WindowPtr window);

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
	 * Renders a scene
	 * @param scene Scene
	 * @param camera Camera
	 */
	void render(const ScenePtr scene, const CameraPtr camera);

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
	 * The current background (clear) color
	 */
	Color _backgroundColor;

	/**
	 * Time (in decimal seconds) at which the last frame was rendered (relative to the start of rendering)
	 */
	float _timeOfLastFrame = 0.f;

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
	 * Traverses the scene graph and builds a render-ready state. This function performs **no drawing**. It strictly
	 * prepares a flattened, pass-local representation of the scene.
	 * @param scene The scene to traverse
	 * @param camera Camera
	 * @return The render state
	 */
	RenderState traverseScene(const ScenePtr scene, const CameraPtr camera);

	/**
	 * Recursively traverses the scene graph to build a render-ready state. Walks the entity hierarchy beginning
	 * at a specified root entity, accumulating world transforms and organizing nodes for the current render pass.
	 * This function performs **no drawing**. It strictly prepares a flattened, pass-local representation of the scene.
	 * @param entity Current entity we are traversing over in the tree
	 * @param parent Parent entity
	 * @param state The render state being populated for this frame and render pass. Meshes, lights, and other renderable
	 * are appended here.
	 */
	void traverseScene(const EntityPtr entity, const EntityPtr parent, RenderState& state);

	/**
	 * Consumes a prepared render state and submits draw calls for all items
	 * @param state The render state to draw
	 */
	void draw(const RenderState& state);
};