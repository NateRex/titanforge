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