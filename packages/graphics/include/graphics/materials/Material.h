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
	 * Color. Defaults to a transparent.
	 */
	const Color color;

	/**
	 * Texture. Can be null.
	 */
	const TexturePtr texture;

protected:

	/**
	 * Constructor
	 * @param type Material type.
	 * @param color Color
	 * @param texture Texture. Can be null.
	 */
	Material(MaterialType type, const Color& color, const TexturePtr texture);

	/**
	 * Builder used to construct a Material instance
	 * @author Nathaniel Rex
	 */
	template <typename SubBuilder>
	class Builder {
	public:

		/**
		 * Sets the color
		 * @param color Color value
		 * @return This builder
		 */
		SubBuilder& setColor(const Color& color)
		{
			_color = color;
			return static_cast<SubBuilder&>(*this);
		}

		/**
		 * Sets the texture
		 * @param texture Texture value
		 * @return This vuilder
		 */
		SubBuilder& setTexture(TexturePtr texture)
		{
			_texture = texture;
			return static_cast<SubBuilder&>(*this);
		}

	protected:

		/**
		 * Color
		 */
		Color _color = Color::TRANSPARENT;

		/**
		 * Texture
		 */
		TexturePtr _texture = nullptr;
	};
};