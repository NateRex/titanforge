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
BOOST_AUTO_TEST_CASE(InputController_actionBindings)
{
	// Record starting mouse position
	InputController* controller = GlobalTestFixture::RENDERER->getWindow()->getInputController();
	controller->processMouseMovement(0.f, 0.f);

	// Create context
	InputContextPtr context = InputContext::create();
	InputAction action("Action", InputValueType::BOOLEAN);
	context->add(DigitalInput::KEY_0, InputTrigger::PRESSED, action);
	context->add(DigitalInput::KEY_1, InputTrigger::PRESSED, action);
	context->add(AxisInput::MOUSE_MOVE, action);
	context->add(AxisInput::MOUSE_SCROLL, action);

	// Add context to controller
	controller->addContext(context);
	
	// Bind action
	int value = 0;
	controller->bind(action, [&value](const InputValue& v, float deltaTime) {
		value++;
	});

	// Trigger via two separate keys, movement of the mouse, and scrolling. Value is now 4.
	controller->processKeyEvent(GLFW_KEY_0, GLFW_PRESS, 0);
	controller->processKeyEvent(GLFW_KEY_1, GLFW_PRESS, 0);
	controller->processMouseMovement(1.f, 1.f);
	controller->processMouseScroll(1.f, 1.f);
	BOOST_TEST(value == 4);

	// Remove bindings
	controller->clearBindings();

	// Can no longer trigger. Value is still 4.
	controller->processKeyEvent(GLFW_KEY_0, GLFW_PRESS, 0);
	controller->processMouseMovement(2.f, 2.f);
	controller->processMouseScroll(2.f, 2.f);
	BOOST_TEST(value == 4);

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
	context->add(DigitalInput::KEY_0, InputTrigger::PRESSED, action);

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