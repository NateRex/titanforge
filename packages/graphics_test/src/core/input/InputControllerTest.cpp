#include <boost/test/unit_test.hpp>
#include <graphics_test/GlobalTestFixture.h>
#include <graphics/core/Renderer.h>
#include <graphics/core/windows/Window.h>
#include <graphics/core/input/InputContext.h>
#include <graphics/core/input/InputController.h>
#include <GLFW/glfw3.h>

/**
 * Tests that actions can be bound and unbound to callback functions via an input controller, which are triggered via
 * key binding events. When actions are unbound, the callback function can no longer be triggered.
 */
BOOST_AUTO_TEST_CASE(InputController_keyBindings)
{
	// Record starting mouse position
	InputController* controller = GlobalTestFixture::RENDERER->getWindow()->getInputController();
	controller->processMouseMovement(0.f, 0.f);

	// Create context with two mappings for the action
	InputContextPtr context = InputContext::create();
	InputAction action("Action", InputValueType::BOOLEAN);
	context->add(InputKey::KEY_0, InputTrigger::PRESSED, action);
	context->add(InputKey::KEY_1, InputTrigger::PRESSED, action);
	context->add(InputKey::MOUSE_MOVE, action);

	// Add context to controller
	controller->addContext(context);
	
	// Bind action
	int value = 0;
	controller->bind(action, [&value](const InputValue& v, float deltaTime) {
		value++;
	});

	// Trigger twice via two separate keys and movement of the mouse. Value is now 3.
	controller->processKeyEvent(GLFW_KEY_0, GLFW_PRESS, 0);
	controller->processKeyEvent(GLFW_KEY_1, GLFW_PRESS, 0);
	controller->processMouseMovement(1.f, 1.f);
	BOOST_TEST(value == 3);

	// Remove binding
	controller->clearBindings();

	// Can no longer trigger. Value is still 3.
	controller->processKeyEvent(GLFW_KEY_0, GLFW_PRESS, 0);
	controller->processMouseMovement(2.f, 2.f);
	BOOST_TEST(value == 3);

	// Cleanup
	controller->clearContexts();
}

/**
 * Tests that contexts can be added and removed from an input controller. When a context is removed, the actions
 * mapped to via that context can no longer be activated.
 */
BOOST_AUTO_TEST_CASE(InputController_addAndRemoveContexts)
{
	// Create context with mapping for one action
	InputAction action("Action", InputValueType::BOOLEAN);
	InputContextPtr context = InputContext::create();
	context->add(InputKey::KEY_0, InputTrigger::PRESSED, action);

	// Add context to controller
	InputController* controller = GlobalTestFixture::RENDERER->getWindow()->getInputController();
	controller->addContext(context);

	// Bind action
	int value = 0;
	controller->bind(action, [&value](const InputValue& v, float deltaTime) {
		value++;
	});

	// Trigger. Value is now 1.
	controller->processKeyEvent(GLFW_KEY_0, GLFW_PRESS, 0);
	BOOST_TEST(value == 1);

	// Remove context
	controller->clearContexts();

	// Can no longer trigger. Value is still 1.
	controller->processKeyEvent(GLFW_KEY_0, GLFW_PRESS, 0);
	BOOST_TEST(value == 1);

	// Cleanup
	controller->clearBindings();
}