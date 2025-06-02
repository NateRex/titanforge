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
	InputActionPtr action1 = InputAction::create("Action1", InputValueType::BOOLEAN);
	InputActionPtr action2 = InputAction::create("Action2", InputValueType::VECTOR_2D);
	InputValuePtr value = InputValue::create(true);

	InputContextPtr context = InputContext::create();
	context->add(InputKey::KEY_SPACE, InputTrigger::PRESSED, action1, value);
	context->add(InputKey::KEY_SPACE, InputTrigger::PRESSED, action2, value);
	context->add(InputKey::KEY_A, InputTrigger::PRESSED, action1, value);
	context->add(InputKey::KEY_SPACE, InputTrigger::RELEASED, action1, value);

	BOOST_TEST(context->size() == 4);
	
	std::vector<InputActionMapping> out;
	context->get(InputKey::KEY_SPACE, InputTrigger::PRESSED, out);
	BOOST_TEST(out.size() == 2);

	out.clear();
	context->get(InputKey::KEY_A, InputTrigger::PRESSED, out);
	BOOST_TEST(out.size() == 1);

	out.clear();
	context->get(InputKey::KEY_SPACE, InputTrigger::RELEASED, out);
	BOOST_TEST(out.size() == 1);

	out.clear();
	context->get(InputKey::KEY_LEFT_SHIFT, InputTrigger::PRESSED, out);
	BOOST_TEST(out.size() == 0);
}