#pragma once
#include <mutex>

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
	 */
	static void start(bool headlessMode = false);

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
	 * Boolean flag that, when true, indicates that the engine has been initialized
	 */
	static bool _INITIALIZED;

	/**
	 * Constructor
	 */
	Engine() = delete;

	/**
	 * Constructor
	 * @param engine Engine to copy from
	 */
	Engine(const Engine& engine) = delete;

	/**
	 * Constructor
	 * @param engine Engine to copy from
	 */
	Engine(Engine&& engine) = delete;

	/**
	 * Helper method that asserts the engine has been initialized. This method should be called
	 * internally prior to creating any additional resources.
	 */
	static void assertInitialized();

	/**
	 * Applies constant global OpenGL settings
	 */
	static void applyGlobalGLSettings();
};