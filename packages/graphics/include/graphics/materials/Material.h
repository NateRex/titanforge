#pragma once
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
	 * Color. Defaults to solid white.
	 */
	Color color = Color::WHITE;

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
	 * Destructor
	 */
	virtual ~Material() = default;

protected:

	/**
	 * Constructor
	 * @param type Material type
	 */
	Material(MaterialType type);
};
