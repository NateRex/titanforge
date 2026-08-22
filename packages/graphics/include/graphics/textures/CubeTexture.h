#pragma once
#include <graphics/textures/pointers/CubeTexturePtr.h>
#include <graphics/core/PixelFormats.h>
#include <graphics/textures/TextureFormats.h>
#include <array>
#include <string>

/**
 * Describes the dimensions, format, and sampling behavior of a cube texture. Every face is assumed to be a square
 * and uses the same size and pixel format.
 * @author Nathaniel Rex
 */
struct CubeTextureConfig
{
	/**
	 * Width and height of each face in pixels
	 */
	unsigned int size = 1;

	/**
	 * Texel storage format shared by all six faces
	 */
	PixelFormat format = PixelFormat::RGBA8;

	/**
	 * Cube texture sampling configuration
	 */
	TextureSampling sampling = {
		TextureFilter::LINEAR,
		TextureFilter::LINEAR,
		TextureWrap::CLAMP_TO_EDGE,
		TextureWrap::CLAMP_TO_EDGE,
		TextureWrap::CLAMP_TO_EDGE,
		{0.f, 0.f, 0.f, 0.f}
	};

	/**
	 * Boolean flag that, when true, triggers generation of a complete mipmap chain. Defaults to false.
	 */
	bool generateMipmaps = false;
};

/**
 * A cube texture that is made up of six images
 * @author Nathaniel Rex
 */
class CubeTexture
{
public:

	/**
	 * Destructor
	 */
	~CubeTexture();

	/**
	 * Creates a cube texture from six image files.
	 * @param paths Image paths, in the order right, left, top, bottom, front, and back
	 * @param flip Boolean flag that, when true, flips each image while loading. Defaults to false.
	 * @return The new cube texture.
	 */
	static CubeTexturePtr create(const std::array<std::string, 6>& paths, bool flip = false);

	/**
	 * Creates a cube texture from a storage config and optional face data.
	 * @param config Face size, format, mipmap behavior, and sampling configuration.
	 * @param data Optional tightly packed pixel data for each face, in the order right, left, top, bottom, front,
	 * and back. Null entries allocate a face without initial pixel values.
	 * @return The new cube texture.
	 */
	static CubeTexturePtr create(const CubeTextureConfig& config, const std::array<void*, 6>& data = {});

	/**
	 * @return The OpenGL object name of this cube texture
	 */
	unsigned int id() const { return _id; }

	/**
	 * @return The width and height of each face in texels
	 */
	unsigned int size() const { return _config.size; }

	/**
	 * @return The pixel format used by every face
	 */
	PixelFormat format() const { return _config.format; }

	/**
	 * @return The complete storage and sampling config for this cube texture
	 */
	const CubeTextureConfig& config() const { return _config; }

	/**
	 * Reallocates all six faces. Existing pixel contents are discarded.
	 * @param size New width and height of each face. Must be greater than zero.
	 */
	void resize(unsigned int size);

private:

	/**
	 * OpenGL ID of this texture
	 */
	unsigned int _id = 0;

	/**
	 * Storage and sampling configuration currently applied to this texture
	 */
	CubeTextureConfig _config;

	/**
	 * Constructor
	 * @param paths Image paths, in the order right, left, top, bottom, front, and back
	 * @param flip Boolean flag that, when true, flips each image while loading. Defaults to false.
	 */
	CubeTexture(const std::array<std::string, 6>& paths, bool flip);
	
	/**
	 * Constructor
	 * @param config Face size, format, mipmap behavior, and sampling configuration.
	 * @param data Optional tightly packed pixel data for each face, in the order right, left, top, bottom, front,
	 * and back. Null entries allocate a face without initial pixel values.
	 */
	CubeTexture(const CubeTextureConfig& config, const std::array<void*, 6>& data);

	/**
	 * Allocates or reallocates storage for all six faces.
	 */
	void allocate(const std::array<void*, 6>& data);

	/**
	 * Applies the sampling configuration to the bound cube texture.
	 */
	void applySampling() const;
};
