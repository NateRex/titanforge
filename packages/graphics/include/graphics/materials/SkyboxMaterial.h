#pragma once
#include <graphics/materials/pointers/SkyboxMaterialPtr.h>
#include <graphics/materials/Material.h>

/**
 * A material containing the uniform values used in skybox shaders
 * @author Nathaniel Rex
 */
class SkyboxMaterial : public Material
{
public:

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
