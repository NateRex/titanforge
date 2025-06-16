#pragma once
#include <graphics/core/input/modifiers/InputModifier.h>
#include <math/Axis.h>
#include <vector>
#include <memory>

 /**
  * A chain of modifiers that transform an input value before it is passed to an action callback. This allows for features
  * like scaling, axis inversion, etc. The operations are performed in the order they were specified.
  * @author Nathaniel Rex
  */
class InputModifiers : public InputModifier
{
public:

	/**
	 * @override
	 */
	InputValue apply(const InputValue& value) const override;

	/**
	 * @return The number of modifications that this input modifier chain will perform on inputs
	 */
	std::size_t size() const;

	/**
	 * Negates input.
	 * @return This modifier instance
	 */
	InputModifiers& negate();

	/**
	 * Negates the x component, or any one dimensional input value (e.g, a boolean or scalar).
	 * @return This modifier instance
	 */
	InputModifiers& negateX();

	/**
	 * Negates the y component. Only has an effect on input values of two or more dimensions (e.g, a 2D vector).
	 * @return This modifier instance
	 */
	InputModifiers& negateY();

	/**
	 * Negates the z component. Only has an effect on input values of three or more dimensions (e.g, a 3D vector).
	 * @return This modifier instance
	 */
	InputModifiers& negateZ();

	/**
	 * Reorders (swizzles) the x, y, and z components of an input value to a given pattern. Only the components that
	 * exist in the input value (based on its type) are used. Invalid accesses (e.g. requesting Y on a boolean or scalar)
	 * are skipped.
	 * @param first Component used for the output X (first) value. Can be X, Y, or Z.
	 * @param second Component used for the output Y (second) value; can be X, Y, or Z.
	 * @param third Component used for the output Z (third) value; can be X, Y, or Z.
	 * @return This modifier instance
	 */
	InputModifiers& swizzle(Axis first, Axis second, Axis third);

private:

	/**
	 * The ordererd list of modifiers
	 */
	std::vector<std::shared_ptr<InputModifier>> _modifiers;
};
