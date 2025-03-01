#pragma once
#include <graphics/window/Window.h>
#include <mutex>
#include <string>

class Buffer;
class IShader;

/**
 * TitanForge engine responsible for maintaining all allocated resources used to render
 * graphics
 * @author Nathaniel Rex
 */
class Engine
{
public:

	/**
	 * Initialize the TitanForge graphics library for use
	 */
	static void start();

	/**
	 * De-initialize the TitanForge graphics library and clear up all resources
	 */
	static void stop();

	/**
	 * Makes a given window the new current context
	 * @param window Window instance 
	 */
	static void setContext(Window& window);

	/**
	 * @return A new window
	 * @throws IllegalStateException If the window could not be created
	 */
	static Window createWindow();

	/**
	 * Constructs a new buffer for the current context
	 * @return A new buffer used to store data to be passed to the GPU
	 * @throws IllegalStateException If the buffer could not be created
	 */
	static Buffer createBuffer();

	/**
	 * Fetches a shader by name. Requires a window to have been created and made active for the current context.
	 * @param name Unique name of the shader. This shader must have been previously registered
	 * with the shader manager in order to be found.
	 * @return A pointer to the shader instance, or null if not found.
	 */
	static const IShader* getShader(const std::string& name);

private:

	/**
	 * Mutex lock for accessing the global engine instance
	 */
	static std::mutex _MUTEX;

	/**
	 * Boolean flag indicating whether or not the graphics engine has been initialized
	 */
	static bool _INIT;

	/**
	 * Window representing the current context
	 */
	static Window _CURRENT_WINDOW;

	/**
	 * Constructor
	 */
	Engine();

	/**
	 * Helper method that asserts the engine has been initialized. This method should be called
	 * prior to creating each resource.
	 */
	static void assertInitialized();

};