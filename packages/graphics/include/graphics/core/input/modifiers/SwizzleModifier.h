#pragma once
#include <graphics/core/input/modifiers/InputModifier.h>

/**
 * Enumerated type defining a reordering of component axes, used in conjunction with a swizzle input modifier.
 * @author Nathaniel Rex
 */
enum class AxisOrder
{
	XYZ,
	XZY,
	YXZ,
	YZX,
	ZXY,
	ZYX
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
	 * Swizzle order, representing the reordering of axis values
	 */
	AxisOrder _order;

	/**
	 * Constructor
	 * @param order Swizzle order, representing the reordering of axis values.
	 */
	SwizzleModifier(AxisOrder order);
};