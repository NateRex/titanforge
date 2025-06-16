#include <graphics/core/input/modifiers/InputModifiers.h>
#include <graphics/core/input/modifiers/NegateModifier.h>
#include <graphics/core/input/modifiers/SwizzleModifier.h>
#include <graphics/core/input/InputValue.h>

InputValue InputModifiers::apply(const InputValue& value) const
{
	InputValue result = value;

	for (auto& mod : _modifiers)
	{
		result = mod->apply(result);
	}

	return result;
}

std::size_t InputModifiers::size() const
{
	return _modifiers.size();
}

InputModifiers& InputModifiers::negate()
{
	_modifiers.push_back(std::make_shared<NegateModifier>(true, true, true));
	return *this;
}

InputModifiers& InputModifiers::negateX()
{
	_modifiers.push_back(std::make_shared<NegateModifier>(true, false, false));
	return *this;
}

InputModifiers& InputModifiers::negateY()
{
	_modifiers.push_back(std::make_shared<NegateModifier>(false, true, false));
	return *this;
}

InputModifiers& InputModifiers::negateZ()
{
	_modifiers.push_back(std::make_shared<NegateModifier>(false, false, true));
	return *this;
}

InputModifiers& InputModifiers::swizzle(Axis first, Axis second, Axis third)
{
	_modifiers.push_back(std::make_shared<SwizzleModifier>(first, second, third));
	return *this;
}