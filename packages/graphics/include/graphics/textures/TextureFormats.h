#pragma once

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
 * Configures filtering, wrapping, and border color for texture sampling.
 * Textures will only use the coordinate axes that apply to them.
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
	 * Wrap mode for the first texture coordinate. Defaults to TextureWrap::REPEAT.
	 */
	TextureWrap sWrap = TextureWrap::REPEAT;

	/**
	 * Wrap mode for the second texture coordinate. Defaults to TextureWrap::REPEAT.
	 */
	TextureWrap tWrap = TextureWrap::REPEAT;

	/**
	 * Wrap mode for the third texture coordinate. Ignored by 1D and 2D textures.
	 * Defaults to TextureWrap::REPEAT.
	 */
	TextureWrap rWrap = TextureWrap::REPEAT;

	/**
	 * Border color used during border wrapping. Defaults to black.
	 */
	float borderColor[4] = {0.f, 0.f, 0.f, 0.f};
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