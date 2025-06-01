#include <graphics/core/input/InputAction.h>

InputAction::InputAction(const std::string& name, const InputValueType valueType) : name(name), valueType(valueType)
{

}

bool InputAction::operator==(const InputAction& other) const
{
	return name == other.name && valueType == other.valueType;
}

bool InputAction::operator!=(const InputAction& other) const
{
	return !(*this == other);
}