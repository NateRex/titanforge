#pragma once
#include <graphics/core/input/InputAction.h>
#include <graphics/core/input/modifiers/InputModifiers.h>

/**
 * Internal structure that maps a key and trigger event to an input action, with optional value modifiers.
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
	 * Modifiers
	 */
	InputModifiers modifiers;
};