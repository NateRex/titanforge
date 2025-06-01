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

	friend class InputContext;

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
	const InputActionPtr action;

	/**
	 * Value
	 */
	const InputValuePtr value;

	/**
	 * Destructor
	 */
	~InputActionMapping();

private:

	/**
	 * Constructor
	 * @param key GLFW key code
	 * @param trigger Trigger event
	 * @param action Action to trigger
	 * @param value Value that should be passed to the executable bound to the action, when triggered
	 */
	InputActionMapping(int key, InputTrigger trigger, const InputActionPtr action, const InputValuePtr value);
};