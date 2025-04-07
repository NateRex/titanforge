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
	const MaterialType type;

	/**
	 * Color. Defaults to solid white.
	 */
	Color color;

	/**
	 * Texture. Can be null.
	 */
	TexturePtr texture;

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