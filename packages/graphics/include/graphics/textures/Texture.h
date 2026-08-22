#pragma once
#include <graphics/textures/pointers/TexturePtr.h>
#include <graphics/core/PixelFormats.h>
#include <graphics/textures/TextureFormats.h>
#include <string>

/**
 * Configures filtering, wrapping, and border color for 2D texture sampling
 * @author Nathaniel Rex
 */
struct TextureSampling
{
	/**
	 * Filter used when the texture is minified. Defaults to TextureFilter::LINEAR.
	 */
	TextureFilter minFilter = TextureFilter::LINEAR;

	/**
	 * Filter used when the texture is magnified. Defaults to TextureFilter::LINEAR.
	 */
	TextureFilter magFilter = TextureFilter::LINEAR;

	/**
	 * Wrap mode for horizontal texture coordinate. Defaults to TextureWrap::REPEAT.
	 */
	TextureWrap sWrap = TextureWrap::REPEAT;

	/**
	 * Wrap mode for veritical texture coordinate. Defaults to TextureWrap::REPEAT.
	 */
	TextureWrap tWrap = TextureWrap::REPEAT;

	/**
	 * Border color used during border wrapping. Defaults to black.
	 */
	float borderColor[4] = {0.f, 0.f, 0.f, 0.f};
};

/**
 * Describes the dimensions, format, and sampling behavior of a 2D texture
 * @author Nathaniel Rex
 */
struct TextureConfig
{
	/**
	 * Texture width in pixels
	 */
	unsigned int width = 1;

	/**
	 * Texture height in pixels
	 */
	unsigned int height = 1;

	/**
	 * Texel storage format
	 */
	PixelFormat format = PixelFormat::RGBA8;

	/**
	 * Texture sampling configuration
	 */
	TextureSampling sampling;

	/**
	 * Boolean flag that, when true, triggers generation of a complete mipmap chain. Defaults to false.
	 */
	bool generateMipmaps = false;
};

/**
 * A 2D texture capable of being sampled
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
	 * @param config Texture dimensions, format, mipmap behavior, and sampling configuration.
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
	 * @return The complete storage and sampling config for this texture
	 */
	const TextureConfig& config() const { return _config; }

	/**
	 * Reallocates texture storage while preserving its format and sampling configuration. Existing pixel contents are discarded.
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
	 * @param config Texture storage and sampling configuration.
	 * @param data Optional tightly packed pixel data. Can be null.
	 */
	Texture(const TextureConfig& config, const void* data);

	/**
	 * Allocates or reallocates the texture's GPU storage.
	 * @param data Optional tightly packed initial pixel data. Can be null.
	 */
	void allocate(const void* data);

	/**
	 * Applies the sampling configuration to the bound texture.
	 */
	void applySampling() const;
};
