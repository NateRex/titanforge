#pragma once
#include <graphics/textures/pointers/CubeTexturePtr.h>
#include <graphics/core/PixelFormats.h>
#include <graphics/textures/TextureFormats.h>
#include <string>

/**
 * Configures filtering, wrapping, and border color for cube texture sampling
 * @author Nathaniel Rex
 */
struct CubeTextureSampling
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
	 * Wrap mode for horizontal texture coordinate. Defaults to TextureWrap::CLAMP_TO_EDGE.
	 */
	TextureWrap sWrap = TextureWrap::CLAMP_TO_EDGE;

	/**
	 * Wrap mode for vertical texture coordinate. Defaults to TextureWrap::CLAMP_TO_EDGE.
	 */
	TextureWrap tWrap = TextureWrap::CLAMP_TO_EDGE;

	/**
	 * Wrap mode for depth texture coordinate. Defaults to TextureWrap::CLAMP_TO_EDGE.
	 */
	TextureWrap rWrap = TextureWrap::CLAMP_TO_EDGE;

	/**
	 * Border color used during border wrapping. Defaults to black.
	 */
	float borderColor[4] = {0.f, 0.f, 0.f, 0.f};
};

/**
 * Describes the dimensions, format, and sampling behavior of a cube texture.
 * Every face is square and uses the same size and pixel format.
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
	CubeTextureSampling sampling;

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

};
