#pragma once
#include <iosfwd>
#include <mutex>
#include <map>

class Texture;

/**
 * The texture manager is responsible for tracking all buffers that have been created
 * @author Nathaniel Rex
 */
class TextureManager
{
public:

	friend class Engine;

	/**
	 * Creates a new texture and saves it in this manager for future use
	 * @param name A unique name that can be used to refer to this texture in the future
	 * @param imagePath Relative path to the image file that will be used to generate the texture.
	 * This path is relative to the directory containing the currently running executable.
	 * @return The created texture
	 * @throws IllegalArgumentException If a texture with that name already exists
	 */
	static Texture* create(const std::string& name, const std::string& imagePath);

	/**
	 * Get the data for a texture that was previously created via this manager
	 * @param name The name of the texture
	 * @return The texture
	 * @throws IllegalArgumentException If a texture with that name could not be found
	 */
	static Texture* get(const std::string& name);

	/**
	 * Destroys a texture that is no longer needed.
	 * @param name The name of the texture to destroy. Must be a texture that was previously constructed
	 * via this manager.
	 * @throws IllegalArgumentException If a texture with that name could not be found
	 */
	static void destroy(const std::string& name);

private:

	/**
	 * Mutex lock for manipulating static data
	 */
	static std::mutex _MUTEX;

	/**
	 * Map containing all textures that have been created
	 */
	static std::map<std::string, Texture> _TEXTURES;

	/**
	 * Constructor
	 */
	TextureManager() = delete;

	/**
	 * Constructor
	 * @param mgr Texture manager to copy from
	 */
	TextureManager(const TextureManager& mgr) = delete;

	/**
	 * Constructor
	 * @param mgr Texture manager to copy from
	 */
	TextureManager(TextureManager&& mgr) = delete;

	/**
	 * Initializes the texture manager
	 */
	static void setup();

	/**
	 * Clears and deletes all textures held by the buffer manager
	 */
	static void clear();
};