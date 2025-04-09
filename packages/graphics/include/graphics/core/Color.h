#pragma once

/**
 * An RGBA color value
 * @author Nathaniel Rex
 */
class Color
{
public:

	/**
	 * Constructs a color representing fully-opaque black
	 */
	Color();

	/**
	 * Constructor
	 * @param color Color to copy values from
	 */
	Color(const Color& color);

	/**
	 * Constructs a color from four floating point values in the range 0 to 1
	 * @param r Red channel value
	 * @param g Green channel value
	 * @param b Blue channel value
	 * @param a Alpha channel value. Defaults to 1 (fully opaque).
	 */
	Color(float r, float g, float b, float a = 1.0f);

	/**
	 * @return The red channel value (a value between 0 and 1)
	 */
	float red() const;

	/**
	 * @return The green channel value (a value between 0 and 1)
	 */
	float green() const;

	/**
	 * @return The blue channel value (a value between 0 and 1)
	 */
	float blue() const;

	/**
	 * @return The alpha channel value (a value between 0 and 1)
	 */
	float alpha() const;

	/**
	 * Creates a new color that is similar to this one, but with the given alpha channel value
	 * @param alpha Alpha channel value (a value between 0 and 1)
	 * @return The new color
	 */
	Color withAlpha(float alpha) const;

	/**
	 * Determines if this color is equal to another color
	 * @param other Color to compare to
	 * @return True if the two colors are equal. Returns false otherwise.
	 */
	bool operator==(const Color& other) const;

	/**
	 * Determines if this color is not equal to another color
	 * @param other Color to compare to
	 * @return True if the two colors are not equal. Returns false otherwise.
	 */
	bool operator!=(const Color& other) const;

private:

	/**
	 * Red channel value (a value between 0 and 1)
	 */
	float _red;

	/**
	 * Green channel value (a value between 0 and 1)
	 */
	float _green;

	/**
	 * Blue channel value (a value between 0 and 1)
	 */
	float _blue;

	/**
	 * Alpha channel value (a value between 0 and 1)
	 */
	float _alpha;

public:

	static const Color TRANSPARENT;
	static const Color BLACK;
	static const Color WHITE;
	static const Color RED;
	static const Color GREEN;
	static const Color BLUE;
};