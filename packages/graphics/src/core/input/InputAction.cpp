#include <graphics/core/input/InputAction.h>

InputAction::InputAction(const std::string& name, const InputValueType valueType) : _name(name), _valueType(valueType)
{

}

bool InputAction::operator==(const InputAction& other) const
{
	return _name == other._name && _valueType == other._valueType;
}

bool InputAction::operator!=(const InputAction& other) const
{
	return !(*this == other);
}