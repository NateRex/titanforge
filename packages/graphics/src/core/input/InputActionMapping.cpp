#include <graphics/core/input/InputActionMapping.h>

InputActionMapping::InputActionMapping(int key, InputTrigger trigger, const InputAction& action, const InputValue& value)
	: key(key), trigger(trigger), action(action), value(value)
{

}
