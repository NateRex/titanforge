#pragma once
#include <graphics/core/input/modifiers/InputModifier.h>

/**
 * Enumerated type used to map axis components to array index values
 * @author Nathaniel Rex
 */
enum class AxisComponent
{
	X = 0,
	Y = 1,
	Z = 2
};

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
	 * @override
	 */
	InputValue apply(const InputValue& value) const override;

private:

	/**
	 * The axis component that x should be assigned to. In cases where an input value
	 */

	/**
	 * Swizzle order, representing the reordering of axis values
	 */
	AxisComponent _order[3];

	/**
	 * Constructor
	 * @param order Swizzle order, representing the reordering of axis values. Only the components that
	 * exist in the input value (based on its type) are used. Invalid accesses (e.g. requesting Z on a
	 * 2D input) are skipped.
	 */
	SwizzleModifier(const AxisComponent (&order)[3]);
};