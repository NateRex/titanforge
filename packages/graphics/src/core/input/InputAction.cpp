#include <graphics/core/input/InputAction.h>

InputAction::InputAction(const std::string& name, const InputEventType type) : name(name), type(type)
{

}

bool InputAction::operator==(const InputAction& other) const
{
	return name == other.name && type == other.type;
}

bool InputAction::operator!=(const InputAction& other) const
{
	return !(*this == other);
}