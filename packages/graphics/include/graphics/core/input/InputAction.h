#pragma once
#include <string>
#include <graphics/core/input/InputTrigger.h>

/**
 * This class is used to define an abstract input action (e.g., "Jump" or "MoveForward") that
 * can be triggered by specific input events such as key presses, releases, or holds.
 * @author Nathaniel Rex
 */
class InputAction
{
public:

	/**
	 * The name of the action (e.g, "Jump" or "MoveForward")
	 */
	const std::string name;

	/**
	 * The type of input event that triggers this action
	 */
	const InputTrigger type;

	/**
	 * Constructor
	 * @param name The name of the action (e.g, "Jump" or "MoveForward")
	 * @param type The type of input event that triggers this action
	 */
	InputAction(const std::string& name, const InputTrigger type);

	/**
	 * Compares this input action to another, testing for equality.
	 * @param other Input action to compare against
	 * @return True if the two input actions are equal. Returns false otherwise.
	 */
	bool operator==(const InputAction& other) const;

	/**
	 * Compares this input action to another, testing for inequality.
	 * @param other Input action to compare against
	 * @return True if the two input actions are not equal. Returns false otherwise.
	 */
	bool operator!=(const InputAction& other) const;
};