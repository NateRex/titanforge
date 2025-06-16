#pragma once
#include <graphics/core/input/modifiers/InputModifier.h>

enum class Axis;

/**
 * An input modifier that reorders (swizzles) the X, Y, and Z components of an input value to a given pattern.
 * This is useful for remapping joystick or mouse input axes without changing having to change the underlying
 * data source.
 * @author Nathaniel Rex
 */
class SwizzleModifier : public InputModifier
{
public:

	/**
	 * Constructs a swizzle modifier using three components, which define how to reorder an input vector.
	 * Only the components that exist in the input value (based on its type) are used. Invalid accesses
	 * (e.g. requesting Y on a boolean or scalar) are skipped.
	 * @param first Component used for the output X (first) value. Can be X, Y, or Z.
	 * @param second Component used for the output Y (second) value; can be X, Y, or Z.
	 * @param third Component used for the output Z (third) value; can be X, Y, or Z.
	 */
	SwizzleModifier(Axis first, Axis second, Axis third);

	/**
	 * @override
	 */
	InputValue apply(const InputValue& value) const override;

private:

	/**
	 * Swizzle order, representing the reordering of axis values
	 */
	Axis _order[3];
};