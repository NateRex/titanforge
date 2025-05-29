#pragma once
#include <string>
#include <graphics/input/InputEventType.h>

/**
 * This class is used to define an abstract input action (e.g., "Jump" or "MoveForward") that
 * can be triggered by specific input events such as key presses, releases, or holds. It is used
 * in conjunction with an input mapping system to decouple physical inputs (like keyboard keys)
 * from gameplay behavior.
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
	const InputEventType type;

	/**
	 * Constructor
	 * @param name The name of the action (e.g, "Jump" or "MoveForward")
	 * @param type The type of input event that triggers this action
	 */
	InputAction(const std::string& name, const InputEventType type);

	/**
	 * Compares this input action to another, testing for equality.
	 * @param other Input action to compare against
	 * @return True if the two input actions are equal. Returns false otherwise.
	 */
	bool operator==(const InputAction& other) const;
};