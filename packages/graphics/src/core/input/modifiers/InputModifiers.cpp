#include <graphics/core/input/modifiers/InputModifiers.h>
#include <graphics/core/input/modifiers/NegateModifier.h>
#include <graphics/core/input/modifiers/SwizzleModifier.h>
#include <graphics/core/input/InputValue.h>

InputModifiers::~InputModifiers()
{
	for (InputModifier* mod : _modifiers)
	{
		delete mod;
	}

	_modifiers.clear();
}

InputValue InputModifiers::apply(const InputValue& value) const
{
	InputValue result = value;

	for (const InputModifier* mod : _modifiers)
	{
		result = mod->apply(result);
	}

	return result;
}

InputModifiers& InputModifiers::negate()
{
	_modifiers.push_back(new NegateModifier(true, true, true));
	return *this;
}

InputModifiers& InputModifiers::negateX()
{
	_modifiers.push_back(new NegateModifier(true, false, false));
	return *this;
}

InputModifiers& InputModifiers::negateY()
{
	_modifiers.push_back(new NegateModifier(false, true, false));
	return *this;
}

InputModifiers& InputModifiers::negateZ()
{
	_modifiers.push_back(new NegateModifier(false, false, true));
	return *this;
}

InputModifiers& InputModifiers::swizzle(Axis first, Axis second, Axis third)
{
	_modifiers.push_back(new SwizzleModifier(first, second, third));
	return *this;
}