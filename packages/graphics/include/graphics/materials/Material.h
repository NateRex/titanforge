#pragma once
#include <graphics/Color.h>

class Texture;

/**
 * Base class for all materials that define the appearance of objects in a scene.
 * @author Nathaniel Rex
 */
class Material
{
public:

	/**
	 * Color. Defaults to a transparent.
	 */
	const Color color;

	/**
	 * Texture. Can be null.
	 */
	const Texture* texture;

protected:

	/**
	 * Constructor
	 * @param color Color
	 * @param texture Texture. Can be null.
	 */
	Material(const Color& color, const Texture* texture);

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
		SubBuilder& setTexture(Texture* texture)
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
		Texture* _texture = nullptr;
	};
};