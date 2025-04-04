#pragma once
#include <iosfwd>
#include <unordered_map>
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
	 * Creates a new window. By default, this window will not be made the target of the current rendering context. That
	 * must be explicitly done via a call to setCurrent().
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

	/**
	 * Destroys a window and releases all of its resources
	 * @param name The name of the window previously created via this manager
	 * @throws IllegalArgumentException if a window with that name could not be found
	 */
	static void destroy(const std::string& name);

private:

	/**
	 * Mutex lock for manipulating static data
	 */
	static std::mutex _MUTEX;

	/**
	 * Map containing all windows that have been created and are currently open
	 */
	static std::unordered_map<std::string, Window> _ALL_WINDOWS;

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
	 * @param mgr Window manager to copy from
	 */
	WindowManager(const WindowManager& mgr) = delete;

	/**
	 * Constructor
	 * @param mgr Window manager to copy from
	 */
	WindowManager(WindowManager&& mgr) = delete;

	/**
	 * Initializes this window manager
	 */
	static void setup();

	/**
	 * Clears and deletes all windows held by this manager
	 */
	static void clear();
};