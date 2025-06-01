#include <boost/test/unit_test.hpp>
#include <graphics/core/input/InputAction.h>
#include <common/PrintHelpers.h>

/**
 * Tests basic construction and field assignment
 */
BOOST_AUTO_TEST_CASE(InputAction_basics)
{
	InputActionPtr action = InputAction::create("Jump", InputValueType::SCALAR);
	BOOST_TEST(action->name == "Jump");
	BOOST_TEST(action->valueType == InputValueType::SCALAR);
}

/**
 * Tests the ability to determine whether two input actions are equal
 */
BOOST_AUTO_TEST_CASE(InputAction_equals)
{
	InputActionPtr action = InputAction::create("Jump", InputValueType::VECTOR_2D);
	BOOST_TEST(*action == *action);
	BOOST_TEST(*action != *InputAction::create("Run", InputValueType::VECTOR_2D));
	BOOST_TEST(*action != *InputAction::create("Jump", InputValueType::VECTOR_3D));
}