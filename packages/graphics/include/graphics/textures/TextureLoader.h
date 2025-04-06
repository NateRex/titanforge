#pragma once
#include <graphics/textures/pointers/TexturePtr.h>
#include <iosfwd>
#include <unordered_map>

/**
 * The texture loader is responsible for creating and caching textures
 * @author Nathaniel Rex
 */
class TextureLoader
{
public:

	/**
	 * Destructor
	 */
	~TextureLoader();

	/**
	 * @return The global texture loader instance
	 */
	static TextureLoader& getInstance();

	/**
	 * Loads a texture from an image located at the given file path, creating it if it does not yet exist
	 * @param path Relative path to the image file that will be used to generate the texture. This path is relative
	 * to the directory containing the currently running executable.
	 * @param flip (Optional) Boolean flag that, when true, will cause the imagery to be flipped. Defaults to false.
	 * @return The texture
	 */
	TexturePtr load(const std::string& path, bool flip = false);

private:

	/**
	 * Mapping from image filepaths to textures that have been created
	 */
	std::unordered_map<std::string, TexturePtr> _textures;

	/**
	 * Constructor
	 */
	TextureLoader();

	/**
	 * Constructor
	 * @param loader Texture loader to copy from
	 */
	TextureLoader(const TextureLoader& loader) = delete;

	/**
	 * Constructor
	 * @param loader Texture loader to copy from
	 */
	TextureLoader(TextureLoader&& loader) = delete;

	/**
	 * Assignment operator
	 * @param loader Texture loader to assign from
	 */
	TextureLoader& operator=(const TextureLoader& loader) = delete;
};