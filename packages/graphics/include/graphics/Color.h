#pragma once

/**
 * An RGBA color value
 * @author Nathaniel Rex
 */
class Color
{
public:

	/**
	 * Red channel value (a value between 0 and 1)
	 */
	const float red;

	/**
	 * Green channel value (a value between 0 and 1)
	 */
	const float green;

	/**
	 * Blue channel value (a value between 0 and 1)
	 */
	const float blue;

	/**
	 * Alpha channel value (a value between 0 and 1)
	 */
	const float alpha;

	/**
	 * Constructs a color from four floating point values in the range 0 to 1
	 * @param r Red channel value
	 * @param g Green channel value
	 * @param b Blue channel value
	 * @param a Alpha channel value
	 */
	static Color fromFloats(float r, float g, float b, float a);

private:

	/**
	 * Constructor
	 * @param r Red channel value (must be between 0 and 1)
	 * @param g Green channel value (must be between 0 and 1)
	 * @param b Blue channel value (must be between 0 and 1)
	 * @param a Alpha channel value (must be between 0 and 1)
	 */
	Color(float r, float g, float b, float a);
};