#pragma once
#include <graphics/core/input/pointers/InputContextPtr.h>
#include <graphics/core/input/DigitalInput.h>
#include <graphics/core/input/AxisInput.h>
#include <graphics/core/input/InputTrigger.h>
#include <unordered_map>
#include <vector>
#include <utility>

struct DigitalMapping;
struct AxisMapping;
class InputAction;
class InputModifiers;

/**
 * Hashing function used for storing (key, trigger) pairs representing digital input
 * @author Nathaniel Rex
 */
struct DigitalInputHash {

	/**
	 * Computes the hash of a digital (key, trigger) pair
	 * @param p Pair containing the input key, followed by the trigger type
	 * @return The resulting hash
	 */
	std::size_t operator()(const std::pair<DigitalInput, InputTrigger>& p) const;
};

/**
 * Groups a set of action mappings into a named context (e.g. Gameplay, Menu). An InputContext allows modular
 * grouping of input bindings and can be activated/deactivated at runtime. Multiple contexts can be active
 * simultaneously inside of an input component.
 * @author Nathaniel Rex
 */
class InputContext
{
public:

	/**
	 * Creates a new input context instance
	 * @return The new input context
	 */
	static InputContextPtr create();

	/**
	 * Registers a new action mapping to this context for an axis input source (e.g., mouse move or
	 * mouse scroll).
	 * @param input Input that triggers the action
	 * @param action Action
	 */
	void add(AxisInput input, const InputAction& action);

	/**
	 * Registers a new action mapping to this context for an axis input source (e.g., mouse move or
	 * mouse scroll).
	 * @param input Input that triggers the action
	 * @param action Action
	 * @param modifiers Value modifiers
	 */
	void add(AxisInput input, const InputAction& action, const InputModifiers& modifiers);

	/**
	 * Registers a new action mapping to this context for a digital input source (e.g., key presses)
	 * @param input Input that triggers the action
	 * @param trigger Trigger event
	 * @param action Action
	 */
	void add(DigitalInput key, InputTrigger trigger, const InputAction& action);

	/**
	 * Registers a new action mapping to this context
	 * @param input Input that triggers the action
	 * @param trigger Trigger event
	 * @param action Action
	 * @param modifiers Value modifiers
	 */
	void add(DigitalInput key, InputTrigger trigger, const InputAction& action, const InputModifiers& modifiers);

	/**
	 * @return The number of action mappings held by this context
	 */
	std::size_t size() const;

	/**
	 * Returns all actions mapped to by digital input key and trigger pairings (e.g., key presses)
	 * @param results Vector in which to store the results
	 */
	void getDigitalMappings(std::vector<DigitalMapping>& results) const;

	/**
	 * Returns the action mappings associated with a specific digital input key and trigger pairing
	 * (e.g., a specific key press)
	 * @param key Input key
	 * @param trigger Trigger event
	 * @param results Vector in which to store the results
	 */
	void getDigitalMappings(DigitalInput key, InputTrigger trigger, std::vector<DigitalMapping>& results) const;

	/**
	 * Returns all actions mapped to by axis inputs (e.g., mouse move or mouse scroll)
	 */
	void getAxisMappings(std::vector<AxisMapping>& results) const;

	/**
	 * Returns the action mappings associated with a specific axis input (e.g., mouse move)
	 * @param input Input type
	 * @param results Vector in which to store the results
	 */
	void getAxisMappings(AxisInput input, std::vector<AxisMapping>& results) const;

private:

	/**
	 * Mapping from (key, trigger) pairs to a list of action mappings. Used for digital inputs (e.g., key presses) that are
	 * activated via a trigger mechanism.
	 */
	std::unordered_map<std::pair<DigitalInput, InputTrigger>, std::vector<DigitalMapping>, DigitalInputHash> _digitalMappings;

	/**
	 * Mapping from keys to a list of action mappings. Used for axis inputs that are not trigger dependent (e.g. - mouse move
	 * or mouse scroll)
	 */
	std::unordered_map<AxisInput, std::vector<AxisMapping>> _axisMappings;

	/**
	 * Constructor
	 */
	InputContext();
};