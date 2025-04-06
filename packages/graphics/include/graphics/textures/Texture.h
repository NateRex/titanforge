#pragma once
#include <graphics/textures/pointers/TexturePtr.h>
#include <string>

/**
 * A texture capable of being loaded and mapped to vertices by the GPU.
 * @author Nathaniel Rex
 */
class Texture
{
public:

	/**
	 * Destructor
	 */
	~Texture();

	/**
	 * Constructs a new texture instance. In order to ensure textures are cached for future use, it is typically
	 * encouraged that callers use the TextureLoader for creating textures, rather than creating them directly.
	 * @param path Relative path to the image file that will be used to generate the texture.
	 * This path is relative to the directory containing the currently running executable.
	 * @param flip (Optional) Boolean flag that, when true, will cause the imagery to be flipped when loading.
	 * Defaults to false
	 * @return The new texture
	 */
	static TexturePtr create(const std::string& path, bool flip = false);

	/**
	 * @return The GLFW id of this texture
	 */
	unsigned int id() const;

private:

	/**
	 * GLFW id of this texture
	 */
	unsigned int _id;

	/**
	 * Constructor
	 * @param path Relative path to the image file that will be used to generate the texture.
	 * This path is relative to the directory containing the currently running executable.
	 * @param flip Boolean flag that, when true, will cause the imagery to be flipped when loading
	 */
	Texture(const std::string& path, bool flip);
};