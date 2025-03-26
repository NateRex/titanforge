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
	float red;

	/**
	 * Green channel value (a value between 0 and 1)
	 */
	float green;

	/**
	 * Blue channel value (a value between 0 and 1)
	 */
	float blue;

	/**
	 * Alpha channel value (a value between 0 and 1)
	 */
	float alpha;

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
	 * @param a Alpha channel value
	 */
	Color(float r, float g, float b, float a);
};