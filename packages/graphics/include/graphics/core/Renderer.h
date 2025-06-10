#pragma once
#include <graphics/core/windows/pointers/WindowPtr.h>
#include <graphics/scene/pointers/ScenePtr.h>
#include <graphics/cameras/pointers/CameraPtr.h>
#include <graphics/entities/Mesh.h>
#include <graphics/core/Color.h>
#include <mutex>

class Matrix4;

/**
 * The renderer is responsible for managing the current render context target and drawing the scene
 * @author Nathaniel Rex
 */
class Renderer
{
public:

	/**
	 * Constructs a new renderer instance. This will automatically result in a new window being created and made the
	 * current context.
	 */
	Renderer();

	/**
	 * Constructs a new renderer instance using an existing window
	 * @param window Window target
	 */
	Renderer(WindowPtr window);

	/**
	 * Destructor
	 */
	~Renderer();

	/**
	 * @return The total amount of time (in decimal seconds) that this renderer has been active for
	 */
	float getTime() const;

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
	 * Recursively renders an entity belonging to the scene, along with all of its children
	 * @param camera Camera
	 * @param entity Entity to render
	 * @param local2World Matrix representing the transformation from local to world space for the entity. This
	 * should be the combined transformations of all parents to the entity.
	 */
	void renderEntity(const CameraPtr camera, const EntityPtr entity, const Matrix4& local2World) const;

	/**
	 * Renders a mesh belonging to the scene
	 * @param camera Camera
	 * @param mesh Mesh to render
	 * @param local2World Matrix representing the transformation from local to world space for the mesh. This
	 * should be the combined transformations of all parents to the entity.
	 */
	void renderMesh(const CameraPtr camera, const MeshPtr mesh, const Matrix4& local2World) const;
};