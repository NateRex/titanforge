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
	 */
	static void start();

	/**
	 * De-initialize the TitanForge graphics library and clear up all resources
	 */
	static void stop();


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
};