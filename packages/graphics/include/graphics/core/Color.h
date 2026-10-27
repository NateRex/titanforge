#pragma once
#include <array>
#include <string>

/**
 * A linear RGB color value with unencoded alpha. All components lie in [0, 1].
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
	 * Constructs a linear RGB color from four floating point values in the range 0 to 1.
	 * No sRGB decoding is performed; use fromSRGB for design-tool colors.
	 * @throws IllegalArgumentException if any component is outside [0, 1]
	 * @param r Red channel value
	 * @param g Green channel value
	 * @param b Blue channel value
	 * @param a Alpha channel value. Defaults to 1 (fully opaque).
	 */
	Color(float r, float g, float b, float a = 1.0f);

	/**
	 * Decodes standard sRGB components to linear RGB, leaving alpha unchanged.
	 * @param r Finite sRGB red in [0, 1]
	 * @param g Finite sRGB green in [0, 1]
	 * @param b Finite sRGB blue in [0, 1]
	 * @param a Finite unencoded alpha in [0, 1], default 1 (opaque)
	 * @return Linear RGB color with the supplied alpha
	 * @throws IllegalArgumentException if any component is non-finite or outside [0, 1]
	 */
	static Color fromSRGB(float r, float g, float b, float a = 1.f);

	/**
	 * Encodes linear RGB to standard sRGB for export or UI, leaving alpha unchanged.
	 * Do not pass the returned RGB to rendering APIs that expect linear inputs.
	 * @return Array in RGBA order, each component in [0, 1]
	 */
	std::array<float, 4> toSRGB() const;

	/**
	 * Decodes an sRGB hex swatch to linear RGB, leaving alpha unencoded.
	 * @param hex Exactly six RRGGBB or eight RRGGBBAA ASCII hex digits, optionally
	 * prefixed by #, case insensitive. Six digits imply alpha 1. No whitespace,
	 * shorthand, or 0x prefix is accepted. Each byte represents a channel in [0, 255].
	 * @return Linear RGB color with alpha equal to the alpha byte divided by 255
	 * @throws IllegalArgumentException for any invalid representation
	 */
	static Color fromSRGBHex(const std::string& hex);

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
