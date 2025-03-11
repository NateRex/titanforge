#pragma once
#include <string>

/**
 * A texture capable of being loaded and mapped to vertices by the GPU
 * @author Nathaniel Rex
 */
class Texture
{
public:

private:

	/**
	 * GLFW id of this texture. Will be zero until the OpenGL resources for this texture are explicitly
	 * constructed via create().
	 */
	unsigned int _id;

	/**
	 * The path to the file containing the imagery for this texture
	 */
	const std::string _imageFile;

	/**
	 * Constructor
	 * @param imageFile Path to an image file that will be used to generate the texture
	 */
	Texture(const char* imageFile);

	/**
	 * Loads the image and constructs the OpenGL resources to represent the texture
	 * @throws InstantiationException on failure to load the image
	 */
	void create();

	/**
	 * Destroys this texture, freeing its resources
	 */
	void destroy();
};