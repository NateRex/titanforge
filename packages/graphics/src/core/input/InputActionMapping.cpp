#include <graphics/core/input/InputActionMapping.h>

InputActionMapping::InputActionMapping(int key, InputTrigger trigger, const InputActionPtr action, const InputValuePtr value)
	: key(key), trigger(trigger), action(action), value(value)
{

}