#pragma once
#include <graphics/materials/pointers/MeshMaterialPtr.h>
#include <graphics/materials/Material.h>

class MeshMaterial : public Material
{
public:

    /**
	 * Controls how alpha is interpreted. AUTO, the default, makes a color with
	 * alpha below 1 blend automatically. Choose MASK for textures containing
	 * hard-edged holes, or an explicit mode when overriding automatic behavior.
	 */
	AlphaMode alphaMode = AlphaMode::AUTO;

	/**
	 * In MASK mode, fragments with alpha below this value are discarded.
	 */
	float alphaCutoff = 0.5f;

	/**
	 * Boolean flag that determines whether both sides of each facet should be renderered. When false,
	 * back-face culling will occur. Defaults to false.
	 */
	bool doubleSided = false;

	/**
	 * Flag that, when true, forces the use of colors specified on geometry vertices (when available). Defaults
	 * to false.
	 */
	bool useVertexColors = false;

	/**
	 * Reflectivity, expressed as a value between 0 and 1, which affects how much light is reflected on surfaces.
	 * The larger the value, the greater the amount of light being reflected.
	 */
	float reflectivity = 0.5f;

	/**
	 * Shine factor, expressed as a value between 0 and 1, which affects the area of highlights on surfaces.
	 * The larger the value, the tighter the highlight area.
	 */
	float shine = 0.6f;

    /**
	 * Diffuse map used to control diffuse-light contribution across a surface. Can be null.
	 */
	TexturePtr diffuseMap = nullptr;

	/**
	 * Specular map used to control specular-highlight intensity across a surface. Can be null.
	 */
	TexturePtr specularMap = nullptr;

    /**
	 * Creates a mesh material.
	 * @return The new mesh material.
	 */
	static MeshMaterialPtr create();

    /**
	 * @return The alpha mode of this material, which resolves to BLEND when AUTO is selected and color alpha is below 1;
	 * OPAQUE otherwise. Explicit modes are returned unchanged.
	 */
	AlphaMode getEffectiveAlphaMode() const;

private:

    /**
     * Constructor
     */
    MeshMaterial();
};