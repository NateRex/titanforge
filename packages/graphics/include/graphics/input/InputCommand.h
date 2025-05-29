#pragma once
#include <graphics/input/InputAction.h>
#include <functional>

using InputCallback = std::function<void()>;

/**
 * An InputCommand binds an abstract input action (e.g, "Jump" or "Shoot") to a concrete callback function
 * to be executed whenever that action is triggered.
 * @author Nathaniel Rex
 */
class InputCommand
{
public:

	/**
	 * Constructor
	 * @param action Input action that triggers this command
	 * @param callback Function to be executed when the input action is triggered
	 */
	InputCommand(const InputAction& action, const InputCallback& callback);

	/**
	 * Executes the bound callback function associated with this command
	 */
	void execute() const;

private:

	/**
	 * Input action that triggers this command
	 */
	const InputAction _action;

	/**
	 * Function to be executed when the input action is triggered
	 */
	const InputCallback _callback;
};