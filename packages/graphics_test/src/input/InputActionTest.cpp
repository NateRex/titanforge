#include <boost/test/unit_test.hpp>
#include <graphics/input/InputAction.h>
#include <common/PrintHelpers.h>

/**
 * Tests basic construction and field assignment
 */
BOOST_AUTO_TEST_CASE(InputAction_basics)
{
	InputAction action("Jump", InputEventType::PRESSED);
	BOOST_TEST(action.name == "Jump");
	BOOST_TEST(action.type == InputEventType::PRESSED);
}

/**
 * Tests the ability to determine whether two input actions are equal
 */
BOOST_AUTO_TEST_CASE(InputAction_equals)
{
	InputAction action("Jump", InputEventType::PRESSED);
	BOOST_TEST(action == action);
	BOOST_TEST(action != InputAction("Run", InputEventType::PRESSED));
	BOOST_TEST(action != InputAction("Jump", InputEventType::HELD));
}