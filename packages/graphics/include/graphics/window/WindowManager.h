#pragma once
#include <iosfwd>
#include <map>
#include <mutex>

class Window;

/**
 * The window manager is responsible for tracking all window contexts that have been created
 * @author Nathaniel Rex
 */
class WindowManager
{
public:

	friend class Engine;

	/**
	 * Creates a new window. This window will automatically be set as the target of the current rendering context
	 * if no other windows previously existed.
	 * @param name Unique name for the window
	 * @param width (Optional) Starting width (in pixels). Defaults to 800.
	 * @param height (Optional) Starting height (in pixels). Defaults to 600.
	 * @return The new window
	 * @throws IllegalArgumentException if a window with that name already exists
	 */
	static Window* create(const std::string& name, unsigned int width = 800, unsigned int height = 600);

	/**
	 * Fetches a window from this manager, given its name
	 * @param name The name of a window previously created via this manager
	 * @return The window
	 * @throws IllegalArgumentException if a window with that name could not be found
	 */
	static Window* get(const std::string& name);

	/**
	 * @return The window representing the current rendering context. Can be null.
	 */
	static Window* getCurrent();

	/**
	 * Marks a window as the new current rendering context
	 * @param name The name of a window previously created via this manager
	 * @throws IllegalArgumentException if a window with that name could not be found
	 */
	static void setCurrent(const std::string& name);

private:

	/**
	 * Mutex lock for manipulating static data
	 */
	static std::mutex _MUTEX;

	/**
	 * Map containing all windows that have been created and are currently open
	 */
	static std::map<std::string, Window> _OPEN_WINDOWS;

	/**
	 * The window that is currently the target of the rendering context. Will
	 * be null if no window is currently set as the current target.
	 */
	static Window* _CURRENT_WINDOW;

	/**
	 * Constructor
	 */
	WindowManager() = delete;

	/**
	 * Constructor
	 * @param mgr Shader manager to copy from
	 */
	WindowManager(const WindowManager& mgr) = delete;

	/**
	 * Constructor
	 * @param mgr Shader manager to copy from
	 */
	WindowManager(WindowManager&& mgr) = delete;

	/**
	 * Initializes this window manager
	 */
	static void setup();

	/**
	 * Iterates over all the windows of this manager, and checks for any that have been closed by the user.
	 * Closed windows will be destroyed and removed from this manager. Ideally, this method should be called at the
	 * very end of each animation frame.
	 */
	static void refresh();

	/**
	 * Clears and deletes all windows held by this manager
	 */
	static void clear();
};