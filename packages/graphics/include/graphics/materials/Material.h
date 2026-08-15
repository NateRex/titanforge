#pragma once
#include <graphics/materials/pointers/MaterialPtr.h>
#include <graphics/materials/MaterialType.h>
#include <graphics/textures/pointers/TexturePtr.h>
#include <graphics/core/Color.h>

/**
 * Base class for all materials that define the appearance of objects in a scene.
 * @author Nathaniel Rex
 */
class Material
{
public:

	/**
	 * Material type
	 */
	const MaterialType materialType;

	/**
	 * Base color. Defaults to solid white.
	 */
	Color color = Color::WHITE;

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
	 * Texture. Can be null.
	 */
	TexturePtr texture = nullptr;

	/**
	 * Diffuse map used to control diffuse-light contribution across a surface. Can be null.
	 */
	TexturePtr diffuseMap = nullptr;

	/**
	 * Specular map used to control specular-highlight intensity across a surface. Can be null.
	 */
	TexturePtr specularMap = nullptr;

	/**
	 * Destructor
	 */
	virtual ~Material() = default;

	/**
	 * Creates a new basic Material instance
	 * @return The new Material instance
	 */
	static MaterialPtr create();

protected:

	/**
	 * Constructor
	 * @param type Material type
	 */
	Material(MaterialType type);
};
