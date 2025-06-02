#pragma once
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
	 * Constructor
	 * @param name The name of the action (e.g, "Jump" or "MoveForward")
	 * @param valueType The type of value that this action generates
	 */
	InputAction(const std::string& name, const InputValueType valueType);

	/**
	 * @return The name of the action (e.g, "Jump" or "MoveForward")
	 */
	std::string getName() const { return _name; }

	/**
	 * @return The type of value that this action generates
	 */
	InputValueType getValueType() const { return _valueType; }

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
	 * The name of the action (e.g, "Jump" or "MoveForward")
	 */
	std::string _name;

	/**
	 * The type of value that this action generates
	 */
	InputValueType _valueType;
};

namespace std {

	/**
	 * Hashing struct for InputAction
	 */
	template <>
	struct hash<InputAction> {

		/**
		 * Hashing function for InputAction
		 * @param action Action
		 * @return Hash result
		 */
		std::size_t operator()(const InputAction& action) const noexcept {
			std::size_t h1 = std::hash<std::string>{}(action.getName());
			std::size_t h2 = std::hash<int>{}(static_cast<int>(action.getValueType()));
			return h1 ^ (h2 << 1);
		}
	};
}