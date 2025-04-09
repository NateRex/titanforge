#pragma once
#include <graphics/core/windows/pointers/WindowPtr.h>
#include <graphics/entities/pointers/MeshPtr.h>
#include <graphics/core/Color.h>

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
	double getTime() const;

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
	 * Renders the given entity
	 * TODO (Nate) - Update this method to only accept a scene, once that class has been created
	 * @param entity The entity to render
	 */
	void render(const MeshPtr entity) const;

private:

	/**
	 * The current window context
	 */
	WindowPtr _window;

	/**
	 * The current background (clear) color
	 */
	Color _backgroundColor;

	/**
	 * Applies global OpenGL settings that should apply to all entities
	 */
	static void applyGlobalDrawSettings();
};