#pragma once
#include <graphics/materials/pointers/MeshMaterialPtr.h>
#include <graphics/materials/Material.h>

/**
 * Refractive index values for common types of real-world materials
 * @author Nathaniel Rex
 */
namespace RefractiveIndex
{
	/**
	 * Refractive index of air
	 */
	constexpr float AIR = 1.f;

	/**
	 * Refractive index of water
	 */
	constexpr float WATER = 1.33f;

	/**
	 * Refractive index of ice
	 */
	constexpr float ICE = 1.309f;

	/**
	 * Refractive index of glass
	 */
	constexpr float GLASS = 1.52f;

	/**
	 * Refractive index of diamond
	 */
	constexpr float DIAMOND = 2.42f;
};

/**
 * Describes how a mesh material interprets its alpha channel
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

/**
 * Material used to render triangulated meshes
 * @author Nathaniel Rex
 */
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
	 * Decimal percentage of the shaded surface color replaced by environment reflection. Defaults to 0.
	 */
	float reflection = 0.f;

	/**
	 * Decimal percentage of the shaded surface color replaced by environment refraction. Defaults to 0.
	 */
	float refraction = 0.f;

	/**
	 * Index of refraction of the material. Only has an effect when refraction is set to a value greater than 0.
	 * For common refractive index values, see RefractiveIndex.
	 */
	float refractiveIndex = RefractiveIndex::GLASS;

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

	bool isTransparent() const override;

private:

    /**
     * Constructor
     */
    MeshMaterial();
};
