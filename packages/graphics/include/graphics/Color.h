#pragma once

/**
 * An RGBA color value
 * @author Nathaniel Rex
 */
class Color
{
public:

	/**
	 * Constructs a color from four floating point values in the range 0 to 1
	 * @param r Red channel value
	 * @param g Green channel value
	 * @param b Blue channel value
	 * @param a Alpha channel value
	 */
	static Color fromFloats(float r, float g, float b, float a);

	/**
	 * @return The red channel value (a value between 0 and 1)
	 */
	float getRed() const;

	/**
	 * @return The green channel value (a value between 0 and 1)
	 */
	float getGreen() const;

	/**
	 * @return The blue channel value (a value between 0 and 1)
	 */
	float getBlue() const;

	/**
	 * @return The alpha channel value (a value between 0 and 1)
	 */
	float getAlpha() const;

private:

	/**
	 * Constructor
	 * @param r Red channel value (must be between 0 and 1)
	 * @param g Green channel value (must be between 0 and 1)
	 * @param b Blue channel value (must be between 0 and 1)
	 * @param a Alpha channel value (must be between 0 and 1)
	 */
	Color(float r, float g, float b, float a);

	/**
	 * Red channel value
	 */
	float _r;

	/**
	 * Green channel value
	 */
	float _g;

	/**
	 * Blue channel value
	 */
	float _b;

	/**
	 * Alpha channel value
	 */
	float _a;
};