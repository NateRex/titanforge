#pragma once
#include <graphics/window/Window.h>
#include <mutex>

class BufferBuilder;

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
	 * @return The time (in double seconds) since the engine has been started
	 * @throws @throws IllegalStateException if the engine has not been started
	 */
	static double getTime();

	/**
	 * @return The window representing the current context
	 * @throws IllegalStateException if the engine has not been started
	 */
	static Window* getCurrentWindow();

	/**
	 * Makes a given window the new current context
	 * @param window Window instance 
	 * @throws IllegalStateException if the engine has not been started
	 */
	static void setCurrentWindow(Window& window);

	/**
	 * @return A new window
	 * @throws IllegalStateException If the window could not be created
	 */
	static Window createWindow();
	
	/**
	 * Performs initialization logic at the start of a new frame. This includes the processing of user inputs, as well
	 * as clearing the window.
	 * @throws IllegalStateException if the engine has not been started
	 */
	static void startFrame();

	/**
	 * Performs finalization logic for rendering a frame. This includes the swapping of the front and back buffers, as
	 * well as the polling for OpenGL events.
	 * @throws IllegalStateException if the engine has not been started
	 */
	static void finishFrame();

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