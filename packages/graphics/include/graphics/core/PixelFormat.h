#pragma once

/**
 * Identifies the channel layout and storage representation of image data
 * @author Nathaniel Rex
 */
enum class PixelFormat
{
	/**
	 * One normalized 8-bit red channel
	 */
    R8,

	/**
	 * Two normalized 8-bit red and green channels
	 */
    RG8,

	/**
	 * Three normalized 8-bit red, green, and blue channels
	 */
    RGB8,

	/**
	 * Four normalized 8-bit red, green, blue, and alpha channels
	 */
	RGBA8,
	
	/**
	 * Three 8-bit color channels stored in the sRGB color space
	 */
	SRGB8,

	/**
	 * Three 8-bit sRGP color channels and one linear 8-bit alpha channel
	 */
	SRGBA8,

	/**
	 * Three 16-bit floating-point color channels
	 */
	RGB16F,

	/**
	 * Four 16-bit floating-point color channels
	 */
	RGBA16F,

	/**
	 * One unsigned 32-bit integer red channel
	 */
	R32UI,

	/**
	 * Packed format with 24 depth bits and 8 stencil bits
	 */
	DEPTH24_STENCIL8,

	/**
	 * One 32-bit floating-point depth channel
	 */
	DEPTH32F
};

/**
 * Describes the OpenGL storage, pixel, and data types for a texture format
 * @author Nathaniel Rex
 */
struct OpenGLPixelFormat
{
	/**
	 * GPU-side texel storage format
	 */
	int internalFormat;

	/**
	 * Channel layout of pixel data supplied to or read from OpenGL
	 */
	unsigned int format;

	/**
	 * Scalar or packed representation of the supplied pixel data
	 */
	unsigned int type;
};

/**
 * Converts an engine pixel format to its OpenGL representation.
 * @param format Pixel format to convert.
 * @return The corresponding OpenGL format information.
 */
OpenGLPixelFormat toGLFormat(const PixelFormat& format);