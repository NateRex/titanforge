#pragma once
#include <graphics/materials/Material.h>
#include <graphics/materials/pointers/PostProcessMaterialPtr.h>

/**
 * A material containing the uniform values used in post-processing shaders
 * @author Nathaniel Rex
 */
class PostProcessMaterial : public Material
{
public:

	/**
	 * Exposure multiplier applied to the input color. Defaults to 1.
	 */
	float exposure = 1.f;

	/**
	 * Color saturation multiplier. Zero produces grayscale and one preserves the input saturation.
	 */
	float saturation = 1.f;

	/**
	 * Contrast multiplier around middle gray. Defaults to 1.
	 */
	float contrast = 1.f;

	/**
	 * Creates a post-processing material.
	 * @return The new post-processing material.
	 */
	static PostProcessMaterialPtr create();

private:
	
	/**
	 * Constructor
	 */
	PostProcessMaterial();
};
