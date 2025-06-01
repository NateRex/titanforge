#include <graphics/core/input/InputAction.h>

InputAction::InputAction(const std::string& name, const InputValueType valueType) : name(name), valueType(valueType)
{

}

InputActionPtr InputAction::create(const std::string& name, const InputValueType valueType)
{
	return std::shared_ptr<InputAction>(new InputAction(name, valueType));
}

bool InputAction::operator==(const InputAction& other) const
{
	return name == other.name && valueType == other.valueType;
}

bool InputAction::operator!=(const InputAction& other) const
{
	return !(*this == other);
}