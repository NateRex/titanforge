#pragma once
#include <graphics/textures/pointers/TexturePtr.h>
#include <graphics/textures/TextureConfig.h>
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
	 * Creates a two-dimensional texture from an image. In order to ensure image textures are cached for future use, it is typically
	 * encouraged that callers use the TextureLoader rather than creating them directly.
	 * @param path Relative path to the image file that will be used to generate the texture.
	 * This path is relative to the directory containing the currently running executable.
	 * @param flip (Optional) Boolean flag that, when true, will cause the imagery to be flipped when loading.
	 * Defaults to false
	 * @return The new texture
	 */
	static TexturePtr create(const std::string& path, bool flip = false);

	/**
	 * Creates a two-dimensional texture from a storage config.
	 * @param config Texture dimensions, format, mipmap behavior, and sampler configuration.
	 * @param data Optional tightly packed pixel data. When null, storage is allocated without initial pixel values.
	 * @return The new texture.
	 */
	static TexturePtr create(const TextureConfig& config, const void* data = nullptr);

	/**
	 * @return The OpenGL object name of this texture
	 */
	unsigned int id() const { return _id; }

	/**
	 * @return The texture width in texels
	 */
	unsigned int width() const { return _config.width; }

	/**
	 * @return The texture height in texels
	 */
	unsigned int height() const { return _config.height; }

	/**
	 * @return The pixel format used by this texture's storage
	 */
	PixelFormat format() const { return _config.format; }

	/**
	 * @return The complete storage and sampler config for this texture
	 */
	const TextureConfig& config() const { return _config; }

	/**
	 * Reallocates texture storage while preserving its format and sampler configuration. Existing pixel contents are discarded.
	 * @param width New width in texels. Must be greater than zero.
	 * @param height New height in texels. Must be greater than zero.
	 */
	void resize(unsigned int width, unsigned int height);

private:

	/**
	 * OpenGL object name of this texture
	 */
	unsigned int _id = 0;

	/**
	 * Storage and sampling configuration currently applied to this texture
	 */
	TextureConfig _config;

	/**
	 * Constructor
	 * @param path Relative path to the image file that will be used to generate the texture.
	 * This path is relative to the directory containing the currently running executable.
	 * @param flip Boolean flag that, when true, will cause the imagery to be flipped when loading
	 */
	Texture(const std::string& path, bool flip);

	/**
	 * Constructs texture storage from a config and optional initial data.
	 * @param config Texture storage and sampler configuration.
	 * @param data Optional tightly packed pixel data. Can be null.
	 */
	Texture(const TextureConfig& config, const void* data);

	/**
	 * Allocates or reallocates the texture's GPU storage.
	 * @param data Optional tightly packed initial pixel data. Can be null.
	 */
	void allocate(const void* data);

	/**
	 * Applies the sampler configuration to the bound texture.
	 */
	void applySampler() const;
};
