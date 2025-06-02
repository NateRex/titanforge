#pragma once
#include <graphics/core/input/InputTrigger.h>
#include <graphics/core/input/InputAction.h>
#include <graphics/core/input/InputValue.h>

/**
 * Maps a key and trigger event to an input action and value, with optional trigger conditions and value modifiers.
 * @author Nathaniel Rex
 */
class InputActionMapping
{
public:

	/**
	 * GLFW key code
	 */
	const int key;

	/**
	 * Trigger event
	 */
	const InputTrigger trigger;

	/**
	 * Action
	 */
	const InputAction action;

	/**
	 * Value
	 */
	const InputValue value;

	/**
	 * Constructor
	 * @param key GLFW key code
	 * @param trigger Trigger event
	 * @param action Action to trigger
	 * @param value Value that should be passed to the executable bound to the action, when triggered
	 */
	InputActionMapping(int key, InputTrigger trigger, const InputAction& action, const InputValue& value);
};
