#pragma once
#include <graphics/core/input/pointers/InputActionPtr.h>
#include <graphics/core/input/InputValueType.h>
#include <string>

/**
 * This class defines an input action and the type of value it expects (boolean, 1D, 2D, or 3D).
 * Input actions are referenced by mappings in an input context and are bound to behavior
 * in an input component.
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
	 * The type of value that this action expects
	 */
	const InputValueType valueType;

	/**
	 * Creates a new input action
	 * @param name The name of the action (e.g, "Jump" or "MoveForward")
	 * @param valueType The type of value that this action expects
	 * @return The resulting input action
	 */
	static InputActionPtr create(const std::string& name, const InputValueType valueType);

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

private:

	/**
	 * Constructor
	 * @param name The name of the action (e.g, "Jump" or "MoveForward")
	 * @param valueType The type of value that this action expects
	 */
	InputAction(const std::string& name, const InputValueType valueType);
};