#pragma once
#include <graphics/core/input/InputAction.h>
#include <graphics/core/input/InputValue.h>

/**
 * Maps a key and trigger event to an input action and value, with optional trigger conditions and value modifiers.
 * @author Nathaniel Rex
 */
struct InputActionMapping
{
	/**
	 * Input key
	 */
	InputKey key;

	/**
	 * Trigger event
	 */
	InputTrigger trigger;

	/**
	 * Action
	 */
	InputAction action;

	/**
	 * Value
	 */
	InputValue value;
};
