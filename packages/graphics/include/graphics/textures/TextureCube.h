#pragma once
#include <graphics/textures/pointers/TextureCubePtr.h>
#include <graphics/textures/Texture.h>
#include <array>
#include <string>

/** Describes a cube texture whose six faces have the same dimensions and format. */
struct TextureCubeConfig : TextureConfig
{
	/**
	 * Texture width and height in texels
	 */
	unsigned int size = 1;

	TextureCubeConfig()
	{
		sampling.sWrap = TextureWrap::CLAMP_TO_EDGE;
		sampling.tWrap = TextureWrap::CLAMP_TO_EDGE;
		sampling.rWrap = TextureWrap::CLAMP_TO_EDGE;
	}
};

/**
 * A texture composed of six square faces that together form a cube
 * @author Nathaniel Rex
 */
class TextureCube : public Texture
{
public:

	/**
	 * Creates a cube texture from a collection of images.
	 * @param paths Relative paths to the image files that will be used to generate the texture. Each path should be releative to the
	 * current executable. The expected image order is right, left, top, bottom, front, and back.
	 * @param flip (Optional) Boolean flag that, when true, will cause each image to be flipped when loading. Defaults to false.
	 * @return The new texture
	 */
	static TextureCubePtr create(const std::array<std::string, 6>& paths, bool flip = false);

	/**
	 * Creates a cube texture from a storage config.
	 * @param config Texture dimensions, format, mipmap behavior, and sampler configuration.
	 * @param data Optional tightly packed pixel data for each face. The expected data order is right, left, top, bottom, front, and back.
	 * When null, storage is allocated without initial pixel values.
	 * @return The new texture.
	 */
	static TextureCubePtr create(const TextureCubeConfig& config, const std::array<void*, 6>& data = {});

	unsigned int width() const override { return _config.size; }

	unsigned int height() const override { return _config.size; }

	unsigned int size() const { return _config.size; }

	const TextureCubeConfig& config() const override { return _config; }

	void resize(unsigned int width, unsigned int height) override;

	/**
	 * Reallocates texture storage while preserving its format and sampling configuration. Existing pixel contents are discarded.
	 * @param size New texture size in texels. Must be greater than zero.
	 */
	void resize(unsigned int size);

private:

	/**
	 * Currently-applied texture configuration
	 */
	TextureCubeConfig _config;

	/**
	 * Constructor
	 * @param paths Relative paths to the image files that will be used to generate the texture. Each path should be releative to the
	 * current executable. The expected order is right, left, top, bottom, back, and front.
	 * @param flip (Optional) Boolean flag that, when true, will cause each image to be flipped when loading. Defaults to false.
	 */
	TextureCube(const std::array<std::string, 6>& paths, bool flip);

	/**
	 * Constructor
	 * @param config Texture dimensions, format, mipmap behavior, and sampler configuration.
	 * @param data Optional tightly packed pixel data for each face. The expected order is right, left, top, bottom, back, front.
	 * When null, storage is allocated without initial pixel values.
	 */
	TextureCube(const TextureCubeConfig& config, const std::array<void*, 6>& data);

	/**
	 * Allocates or reallocates the GPU storage for this texture using the current configuration. The previously-bound texture
	 * will be reactivated upon completion.
	 * @param data Optional data to upload.The expected order is right, left, top, bottom, back, front.
	 */
	void allocate(const std::array<void*, 6>& data);
};
