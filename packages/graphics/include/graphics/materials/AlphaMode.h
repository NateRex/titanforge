#pragma once

/**
 * Describes how a material interprets its alpha channel
 * @author Nathaniel Rex
 */
enum class AlphaMode
{
	/**
	 * Automatically blends for materials whose color alpha is below 1; otherwise renders opaque.
	 */
	AUTO,

	/**
	 * Ignore alpha for visibility depth testing and render every fragment as solid.
	 */
	OPAQUE,

	/**
	 * Discard fragments below the material alpha cutoff and render the rest as solid.
	 */
	MASK,

	/**
	 * Blend every fragment with the color already in the framebuffer.
	 */
	BLEND
};
