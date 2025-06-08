#include <boost/test/unit_test.hpp>
#include <graphics/core/input/InputContext.h>
#include <graphics/core/input/InputActionMapping.h>
#include <graphics/core/input/InputAction.h>
#include <graphics/core/input/InputValue.h>

/**
 * Tests the ability to add and fetch mappings from an input context
 */
BOOST_AUTO_TEST_CASE(InputContext_addAndFetch)
{
	InputAction action1("Action1", InputValueType::BOOLEAN);
	InputAction action2("Action2", InputValueType::BOOLEAN);
	InputValue value(true);

	InputContextPtr context = InputContext::create();
	context->add(InputKey::KEY_SPACE, InputTrigger::PRESSED, action1, value);
	context->add(InputKey::KEY_SPACE, InputTrigger::PRESSED, action2, value);
	context->add(InputKey::KEY_A, InputTrigger::PRESSED, action1, value);
	context->add(InputKey::KEY_SPACE, InputTrigger::RELEASED, action1, value);

	std::vector<InputActionMapping> allMappings;
	context->getMappings(allMappings);
	BOOST_TEST(context->size() == 4);
	BOOST_TEST(allMappings.size() == 4);
	
	std::vector<InputActionMapping> mappingsForPair;
	context->getMappings(InputKey::KEY_SPACE, InputTrigger::PRESSED, mappingsForPair);
	BOOST_TEST(mappingsForPair.size() == 2);

	mappingsForPair.clear();
	context->getMappings(InputKey::KEY_A, InputTrigger::PRESSED, mappingsForPair);
	BOOST_TEST(mappingsForPair.size() == 1);

	mappingsForPair.clear();
	context->getMappings(InputKey::KEY_SPACE, InputTrigger::RELEASED, mappingsForPair);
	BOOST_TEST(mappingsForPair.size() == 1);

	mappingsForPair.clear();
	context->getMappings(InputKey::KEY_LEFT_SHIFT, InputTrigger::PRESSED, mappingsForPair);
	BOOST_TEST(mappingsForPair.size() == 0);
}