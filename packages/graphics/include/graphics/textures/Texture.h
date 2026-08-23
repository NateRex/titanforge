#pragma once
#include <graphics/textures/pointers/TexturePtr.h>
#include <graphics/textures/TextureFormats.h>
#include <graphics/core/PixelFormats.h>
#include <functional>

/**
 * Common storage and sampling configuration shared by all texture types
 * @author Nathaniel Rex
 */
struct TextureConfig
{
	/**
	 * Texel storage format
	 */
	PixelFormat format = PixelFormat::RGBA8;

	/**
	 * Texture sampling configuration
	 */
	TextureSampling sampling{};

	/**
	 * Boolean flag that, when true, triggers generation of a complete mipmap chain. Defaults to false.
	 */
	bool generateMipmaps = false;
};

/**
 * Abstract base for all textures
 * @author Nathaniel Rex
 */
class Texture
{
public:

	/**
	 * Destructor
	 */
	virtual ~Texture();

	/**
	 * @return The OpenGL object name of this texture
	 */
	unsigned int id() const { return _id; }

	/**
	 * @return The pixel format used by this texture's storage
	 */
	PixelFormat format() const { return config().format; }

	/**
	 * @return The complete storage and sampling config for this texture
	 */
	virtual const TextureConfig& config() const = 0;

	/**
	 * @return The texture width in texels
	 */
	virtual unsigned int width() const = 0;

	/**
	 * @return The texture height in texels
	 */
	virtual unsigned int height() const = 0;

	/**
	 * Reallocates texture storage while preserving its format and sampling configuration. Existing pixel contents are discarded.
	 * @param width New width in texels. Must be greater than zero.
	 * @param height New height in texels. Must be greater than zero.
	 */
	virtual void resize(unsigned int width, unsigned int height) = 0;

protected:

	/**
	 * OpenGL object name of this texture
	 */
	unsigned int _id = 0;

	/**
	 * OpenGL texture type
	 */
	unsigned int _type = 0;

	/**
	 * OpenGL texture binding query
	 */
	unsigned int _binding = 0;

	/**
	 * Storage and sampling configuration currently applied to this texture
	 * @param type OpenGL texture type
	 * @param binding OpenGL texture binding query
	 */
	Texture(unsigned int type, unsigned int binding);

	/**
	 * Allocates or reallocates the GPU storage for this texture using the current configuration. The previously-bound texture
	 * will be reactivated upon completion.
	 * @param includeRWrap Boolean flag that, when true, applies the wrap mode for the third texture coordinate. This should
	 * only be applied for three-dimensional textures.
	 * @param upload Callback function to upload texture data, assuming the texture is bound
	 */
	void allocate(bool includeRWrap, const std::function<void()>& upload);
};
