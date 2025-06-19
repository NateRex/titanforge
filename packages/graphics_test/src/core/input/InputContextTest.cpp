#include <boost/test/unit_test.hpp>
#include <graphics/core/input/InputContext.h>
#include <graphics/core/input/InputContextMappings.h>
#include <graphics/core/input/InputAction.h>

/**
 * Tests the ability to add and fetch digital key mappings
 */
BOOST_AUTO_TEST_CASE(InputContext_digital)
{
	InputAction action1("Action1", InputValueType::BOOLEAN);
	InputAction action2("Action2", InputValueType::BOOLEAN);

	// Exercise the different add() methods
	InputContextPtr context = InputContext::create();
	context->add(DigitalInput::KEY_SPACE, InputTrigger::PRESSED, action1);
	context->add(DigitalInput::KEY_SPACE, InputTrigger::PRESSED, action2);
	context->add(DigitalInput::KEY_A, InputTrigger::PRESSED, action1, InputModifiers().negate());
	context->add(DigitalInput::KEY_SPACE, InputTrigger::RELEASED, action1);

	std::vector<DigitalMapping> allMappings;
	context->getDigitalMappings(allMappings);
	BOOST_TEST(context->size() == 4);
	BOOST_TEST(allMappings.size() == 4);
	
	std::vector<DigitalMapping> specificMappings;
	context->getDigitalMappings(DigitalInput::KEY_SPACE, InputTrigger::PRESSED, specificMappings);
	BOOST_TEST(specificMappings.size() == 2);

	specificMappings.clear();
	context->getDigitalMappings(DigitalInput::KEY_A, InputTrigger::PRESSED, specificMappings);
	BOOST_TEST(specificMappings.size() == 1);
	BOOST_TEST(specificMappings[0].modifiers.size() == 1);

	specificMappings.clear();
	context->getDigitalMappings(DigitalInput::KEY_SPACE, InputTrigger::RELEASED, specificMappings);
	BOOST_TEST(specificMappings.size() == 1);

	specificMappings.clear();
	context->getDigitalMappings(DigitalInput::KEY_LEFT_SHIFT, InputTrigger::PRESSED, specificMappings);
	BOOST_TEST(specificMappings.size() == 0);
}

/**
 * Tests the ability to add and fetch axis input mappings
 */
BOOST_AUTO_TEST_CASE(InputContextTest_axis)
{
	InputAction action1("Action1", InputValueType::BOOLEAN);
	InputAction action2("Action2", InputValueType::BOOLEAN);

	// Exercise the different add() methods
	InputContextPtr context = InputContext::create();
	context->add(AxisInput::MOUSE_MOVE, action1);
	context->add(AxisInput::MOUSE_MOVE, action2);
	context->add(AxisInput::MOUSE_SCROLL, action1, InputModifiers().negate());

	std::vector<AxisMapping> allMappings;
	context->getAxisMappings(allMappings);
	BOOST_TEST(context->size() == 3);
	BOOST_TEST(allMappings.size() == 3);

	std::vector<AxisMapping> specificMappings;
	context->getAxisMappings(AxisInput::MOUSE_MOVE, specificMappings);
	BOOST_TEST(specificMappings.size() == 2);

	specificMappings.clear();
	context->getAxisMappings(AxisInput::MOUSE_SCROLL, specificMappings);
	BOOST_TEST(specificMappings.size() == 1);
	BOOST_TEST(specificMappings[0].modifiers.size() == 1);
}