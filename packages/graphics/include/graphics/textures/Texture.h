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

private:

	/**
	 * GLFW id of this texture
	 */
	unsigned int _id;

	/**
	 * Unique name of this texture
	 */
	const std::string _name;

	/**
	 * The path to the file containing the imagery for this texture. This is relative to
	 * the path stored in the ASSETS_FOLDER environment variable.
	 */
	const std::string _relPath;

	/**
	 * Constructor
	 * @param name A unique name that can be used to refer to this texture in the future
	 * @param relPath Path to the image file that will be used to generate the texture. This must
	 * be relative to the path stored in the ASSETS_FOLDER environment variable.
	 */
	Texture(const std::string& name, const std::string& imageFile);

	/**
	 * Creates the OpenGL resources for this texture
	 */
	void create();

	/**
	 * Destroys this texture, freeing its resources
	 */
	void destroy();
};