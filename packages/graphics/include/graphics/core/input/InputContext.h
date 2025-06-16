#pragma once
#include <graphics/core/input/pointers/InputContextPtr.h>
#include <graphics/core/input/InputAction.h>
#include <graphics/core/input/InputValue.h>
#include <graphics/core/input/InputKey.h>
#include <graphics/core/input/InputTrigger.h>
#include <unordered_map>
#include <vector>
#include <utility>

struct InputActionMapping;
class InputModifiers;

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
	 * Registers a new action mapping to this context
	 * @param key Input key that triggers the action
	 * @param trigger Trigger event
	 * @param action Action
	 */
	void add(InputKey key, InputTrigger trigger, const InputAction& action);

	/**
	 * Registers a new action mapping to this context
	 * @param key Input key that triggers the action
	 * @param trigger Trigger event
	 * @param action Action
	 * @param modifiers Value modifiers
	 */
	void add(InputKey key, InputTrigger trigger, const InputAction& action, const InputModifiers& modifiers);

	/**
	 * @return The number of action mappings held by this context
	 */
	std::size_t size() const;

	/**
	 * Returns all action mappings stored within this context
	 * @param results Vector in which to store the results
	 */
	void getMappings(std::vector<InputActionMapping>& results) const;

	/**
	 * Identifies the action mappings associated with a single input key and trigger pair
	 * @param key Input key
	 * @param trigger Trigger event
	 * @param results Vector in which to store the results
	 */
	void getMappings(InputKey key, InputTrigger trigger, std::vector<InputActionMapping>& results) const;

private:

	/**
	 * Hashing function used for storing (key, trigger) pairs inside of an std::map
	 * @author Nathaniel Rex
	 */
	struct PairHash {

		/**
		 * Computes the hash of a (key, trigger) pair
		 * @param p Pair containing the input key, followed by the trigger type
		 * @return The resulting hash
		 */
		std::size_t operator()(const std::pair<InputKey, InputTrigger>& p) const;
	};

	/**
	 * Mapping from (key, trigger) pairs to a list of action mappings
	 */
	std::unordered_map<std::pair<InputKey, InputTrigger>, std::vector<InputActionMapping>, PairHash> _mappings;

	/**
	 * Constructor
	 */
	InputContext();
};