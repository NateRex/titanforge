#pragma once
#include <graphics/window/Window.h>
#include <mutex>
#include <string>

class Buffer;
class Shader;

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
	 * @param headlessMode Boolean flag that, when true, prevents windows from opening by setting the GLFW window hint
	 * 'GLFW_VISIBLE'. Defaults to false.
	 * @return The starting window context
	 */
	static Window start(bool headlessMode = false);

	/**
	 * De-initialize the TitanForge graphics library and clear up all resources
	 */
	static void stop();

	/**
	 * @return The window representing the current context
	 */
	static Window* getCurrentWindow();

	/**
	 * Makes a given window the new current context
	 * @param window Window instance 
	 */
	static void setCurrentWindow(Window& window);

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
	static const Shader* getShader(const std::string& name);
	
	/**
	 * Renders the next frame for the currently active window context
	 */
	static void renderFrame();

private:

	/**
	 * Mutex lock used for initialization and de-initialization of the engine
	 */
	static std::mutex _MUTEX;

	/**
	 * The global engine instance
	 */
	static Engine* _ENGINE;

	/**
	 * Window representing the current context
	 */
	Window _currentWindow;

	/**
	 * Constructor
	 */
	Engine();

	/**
	 * Helper method that asserts the engine has been initialized. This method should be called
	 * internally prior to creating any additional resources.
	 */
	static void assertInitialized();

};