#pragma once
#include <mutex>

class Buffer;
class Window;

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
	 * @return A new window
	 */
	static Window createWindow();

	/**
	 * @return A new buffer used to store data to be passed to the GPU
	 */
	static Buffer createBuffer();

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
	 * Constructor
	 */
	Engine();

	/**
	 * Helper method that asserts the engine has been initialized. This method should be called
	 * prior to creating each resource.
	 */
	static void assertInitialized();

	/**
	 * Helper method that asserts that a window has been assigned to the current context
	 */
	static void assertWindowContext();

};