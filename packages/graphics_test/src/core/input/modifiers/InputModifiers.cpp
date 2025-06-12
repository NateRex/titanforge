#include <graphics/core/input/modifiers/InputModifiers.h>
#include <graphics/core/input/InputValue.h>

InputValue InputModifiers::apply(const InputValue& value) const
{
	InputValue result = value;

	for (const InputModifier& mod : _modifiers)
	{
		result = mod.apply(value);
	}

	return result;
}