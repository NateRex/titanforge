#pragma once
#include <graphics/core/input/InputTrigger.h>
#include <graphics/core/input/InputAction.h>
#include <graphics/core/input/modifiers/InputModifiers.h>

enum class DigitalInput;
enum class AxisInput;

/**
 * Internal structure that maps a digital key and trigger event to an action, with optional value modifiers.
 * @author Nathaniel Rex
 */
struct DigitalMapping
{
	/**
	 * Input key
	 */
	DigitalInput key;

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

/**
 * Internal structure that maps an axis input to an action, with optional value modifiers.
 * @author Nathaniel Rex
 */
struct AxisMapping
{
	/**
	 * Input
	 */
	AxisInput key;

	/**
	 * Action
	 */
	InputAction action;

	/**
	 * Modifiers
	 */
	InputModifiers modifiers;
};