#pragma once
#include <graphics/core/input/modifiers/InputModifier.h>
#include <vector>

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

private:

	/**
	 * The ordererd list of modifiers
	 */
	std::vector<InputModifier> _modifiers;
};

