#pragma once

class InputValue;

/**
 * Base class for all input modifiers capable of being applied to an input value. Implementations of this class
 * can be chained together as part of an input modifiers chain.
 * @author Nathaniel Rex
 */
class InputModifier
{
public:

	/**
	 * Destructor
	 */
	virtual ~InputModifier() = default;

	/**
	 * Applies this modifier to a given input value
	 * @param value The value to transform
	 */
	virtual InputValue apply(const InputValue& value) const = 0;
};