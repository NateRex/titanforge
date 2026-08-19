#pragma once
#include <graphics/core/PixelFormat.h>

/**
 * Identifies the filtering mode used when sampling textures
 * @author Nathaniel Rex
 */
enum class TextureFilter
{
	/**
	 * Selects the texel nearest to the sampled coordinate
	 */
	NEAREST,

	/**
	 * Linearly interpolates neighboring texels
	 */
	LINEAR,

	/**
	 * Samples the nearest texel from the nearest mipmap level
	 */
	NEAREST_MIPMAP_NEAREST,

	/**
	 * Linearly interpolates texels and adjacent mipmap levels
	 */
	LINEAR_MIPMAP_LINEAR
};

/**
 * Identifies how texture coordinates outside the normalized range are handled
 * @author Nathaniel Rex
 */
enum class TextureWrap
{
	/**
	 * Repeats the texture at every integer coordinate boundary
	 */
	REPEAT,

	/**
	 * Repeats the texture, mirroring every other repetition
	 */
	MIRRORED_REPEAT,

	/**
	 * Clamps coordinates to the texture's edge texels
	 */
	CLAMP_TO_EDGE,

	/**
	 * Uses the configured border color outside the texture
	 */
	CLAMP_TO_BORDER
};

/**
 * Configures filtering, wrapping, and border color for texture sampling
 * @author Nathaniel Rex
 */
struct SamplerDescriptor
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
 * Describes the dimensions, format, and sampling behavior of a texture
 * @author Nathaniel Rex
 */
struct TextureDescriptor
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
	SamplerDescriptor sampler;

	/**
	 * Boolean flag that, when true, triggers generation of a complete mipmap chain. Defaults to false.
	 */
	bool generateMipmaps = false;
};

/**
 * Converts an engine texture filter to its OpenGL representation
 * @param filter Texture filter to convert
 * @return The corresponding OpenGL filter information
 */
unsigned int toGLFilter(const TextureFilter& filter);

/**
 * Converts an engine texture wrap to its OpenGL representation
 * @param wrap Wrap type to convert
 * @return The corresponding OpenGL wrap information
 */
unsigned int toGLWrap(const TextureWrap& wrap);