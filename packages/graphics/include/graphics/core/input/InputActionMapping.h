#pragma once
#include <graphics/core/input/InputTrigger.h>
#include <graphics/core/input/pointers/InputActionPtr.h>
#include <graphics/core/input/pointers/InputValuePtr.h>

/**
 * Maps a key and trigger event to an input action and value, with optional trigger conditions and value modifiers.
 * @author Nathaniel Rex
 */
class InputActionMapping
{
public:

	/**
	 * Constructor
	 * @param key GLFW key code
	 * @param trigger Trigger event
	 * @param action Action to trigger
	 * @param value Value that should be passed to the executable bound to the action, when triggered
	 */
	InputActionMapping(int key, InputTrigger trigger, const InputActionPtr action, const InputValuePtr value);

	/**
	 * @return The GLFW key code
	 */
	int getGLFWKey() { return key; }

	/**
	 * @return The trigger event
	 */
	InputTrigger getTrigger() { return trigger; }

	/**
	 * @return The action
	 */
	InputActionPtr getAction() { return action; }

	/**
	 * @return The value
	 */
	InputValuePtr getValue() { return value; }

private:

	/**
	 * GLFW key code
	 */
	int key;

	/**
	 * Trigger event
	 */
	InputTrigger trigger;

	/**
	 * Action
	 */
	InputActionPtr action;

	/**
	 * Value
	 */
	InputValuePtr value;
};
