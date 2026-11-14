#pragma once
#include <graphics/materials/Material.h>
#include <graphics/materials/pointers/PostProcessMaterialPtr.h>

/**
 * A material containing the uniform values used in post-processing shaders.
 * Input textures, tint RGB, and all effect calculations use linear RGB. Output stays linear
 * for subsequent effects; Renderer applies displayGamma and sRGB encoding only at presentation.
 * Alpha is unencoded and preserved except for multiplication by the material tint alpha.
 * @author Nathaniel Rex
 */
class PostProcessMaterial : public Material
{
public:

	/**
	 * Gain multiplier applied to linear input RGB. Defaults to 1. Applied before this effect's saturation,
	 * contrast, and tint; output remains linear HDR for subsequent effects. Renderer-wide exposure instead
	 * applies after the complete effects chain and before tone mapping at final presentation.
	 */
	float gain = 1.f;

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
	static PostProcessMaterialPtr create()
	{
		return std::shared_ptr<PostProcessMaterial>(new PostProcessMaterial());
	}

private:
	
	/**
	 * Constructor
	 */
	PostProcessMaterial() : Material(MaterialType::POST_PROCESS, ShaderId::POST_PROCESS) {
		cullingMode = CullingMode::NONE;
		depthWrite = false;
		depthFunction = DepthFunction::ALWAYS;
		castShadow = false;
		receiveShadow = false;
	}
};
