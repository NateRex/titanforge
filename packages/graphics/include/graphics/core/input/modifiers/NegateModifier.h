#pragma once
#include <graphics/core/input/modifiers/InputModifier.h>

/**
 * An input modifier that negates one or more components of a value.
 * @author Nathaniel Rex
 */
class NegateModifier : public InputModifier
{
public:

	/**
	 * Constructor
	 * @param x True if the x component should be negated. In cases where only a boolean or scalar is
	 * present, this flag dictates whether or not negation will occur.
	 * @param y True if the y component should be negated. Has no effect on one dimensional values.
	 * @param z True if the z component should be negated. Has no effect on one-dimensional or
	 * two-dimensional values.
	 */
	NegateModifier(bool x, bool y, bool z);

	/**
	 * @override
	 */
	InputValue apply(const InputValue& value) const override;

private:

	/**
	 * Boolean flag that, when true, negates the x component. In cases where only a boolean or scalar is
	 * present, this flag dictates whether or not negation will occur.
	 */
	bool _negateX;

	/**
	 * Boolean flag that, when true, negates the y component. Has no effect on one-dimensional values.
	 */
	bool _negateY;

	/**
	 * Boolean flag that, when true, negates the z component. Has no effect on one-dimensional or
	 * two-dimensional values.
	 */
	bool _negateZ;
};