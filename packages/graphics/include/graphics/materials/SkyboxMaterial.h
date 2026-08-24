#pragma once
#include <graphics/materials/pointers/SkyboxMaterialPtr.h>
#include <graphics/materials/Material.h>
#include <graphics/textures/pointers/TextureCubePtr.h>

/**
 * A material containing the uniform values used in skybox shaders
 * @author Nathaniel Rex
 */
class SkyboxMaterial : public Material
{
public:

	/**
	 * Linear multiplier applied to the sampled skybox texture color
	 */
	float intensity = 1.f;

	/**
	 * Rotation around the world-up axis, in radians
	 */
	float rotation = 0.f;

	/**
	 * Explicit cubemap mip level. Values above zero provide blur when mipmaps exist.
	 */
	float lod = 0.f;

	/**
	 * Creates a skybox material.
	 * @return The new skybox material.
	 */
	static SkyboxMaterialPtr create();

private:
	
	/**
	 * Constructor
	 */
	SkyboxMaterial();
};
