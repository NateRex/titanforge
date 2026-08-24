#pragma once
#include <graphics/textures/pointers/Texture2DPtr.h>
#include <graphics/textures/Texture.h>
#include <string>

/**
 * Describes a two-dimensional texture
 * @author Nathaniel Rex
 */
struct Texture2DConfig : TextureConfig
{
	/**
	 * Texture width in texels
	 */
	unsigned int width = 1;

	/**
	 * Texture height in texels
	 */
	unsigned int height = 1;
};

/**
 * A two-dimensional texture capable of being sampled.
 * @author Nathaniel Rex
 */
class Texture2D : public Texture
{
public:

	/**
	 * Creates a two-dimensional texture from an image. In order to ensure image textures are cached for future use, it is typically
	 * encouraged that callers use the TextureLoader for constructing two-dimensional textures, rather than creating them directly.
	 * @param path Path to the image file. This path should be relative to the current executable.
	 * @param flip (Optional) Boolean flag that, when true, will cause the imagery to be flipped when loading.
	 * Defaults to false
	 * @return The new texture
	 */
	static Texture2DPtr create(const std::string& path, bool flip = false);

	/**
	 * Creates a two-dimensional texture from a storage config.
	 * @param config Texture dimensions, format, mipmap behavior, and sampler configuration.
	 * @param data Optional tightly packed pixel data. When null, storage is allocated without initial pixel values.
	 * @return The new texture.
	 */
	static Texture2DPtr create(const Texture2DConfig& config, const void* data = nullptr);

	unsigned int width() const override { return _config.width; }

	unsigned int height() const override { return _config.height; }

	const Texture2DConfig& config() const override { return _config; }

	void resize(unsigned int width, unsigned int height) override;

private:

	/**
	 * Currently-applied texture configuration
	 */
	Texture2DConfig _config;

	/**
	 * Constructor
	 * @param path Relative path to the image file that will be used to generate the texture.
	 * This path is relative to the directory containing the currently running executable.
	 * @param flip (Optional) Boolean flag that, when true, will cause the imagery to be flipped when loading.
	 * Defaults to false
	 */
	Texture2D(const std::string& path, bool flip);

	/**
	 * Constructor
	 * @param config Texture dimensions, format, mipmap behavior, and sampler configuration.
	 * @param data Optional tightly packed pixel data. When null, storage is allocated without initial pixel values.
	 * @return The new texture.
	 */
	Texture2D(const Texture2DConfig& config, const void* data);

	/**
	 * Allocates or reallocates the GPU storage for this texture using the current configuration. The previously-bound texture
	 * will be reactivated upon completion.
	 * @param data Optional data to upload. Can be null.
	 */
	void allocate(const void* data);
};
