#pragma once
#include <mutex>
#include <string>

class Buffer;
class Window;
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
	 * @return A new window
	 * @throws IllegalStateException If the window could not be created
	 */
	static Window createWindow();

	/**
	 * @return A new buffer used to store data to be passed to the GPU
	 * @throws IllegalStateException If the buffer could not be created
	 */
	static Buffer createBuffer();

	/**
	 * Fetches a shader by name
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