#pragma once
#include <string>

/**
 * A texture capable of being loaded and mapped to vertices by the GPU.
 * @author Nathaniel Rex
 */
class Texture
{
public:

	friend class TextureManager;

	/**
	 * Unique name of this texture
	 */
	const std::string name;

	/**
	 * Relative path to the image file that will be used to generate the texture.
	 * This path is relative to the directory containing the currently running executable.
	 */
	const std::string imagePath;

private:

	/**
	 * GLFW id of this texture
	 */
	unsigned int _id;

	/**
	 * Constructor
	 * @param name A unique name that can be used to refer to this texture in the future
	 * @param imagePath Relative path to the image file that will be used to generate the texture.
	 * This path is relative to the directory containing the currently running executable.
	 */
	Texture(const std::string& name, const std::string& imagePath);

	/**
	 * Creates the OpenGL resources for this texture
	 */
	void create();

	/**
	 * Destroys this texture, freeing its resources
	 */
	void destroy();
};