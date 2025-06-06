#include <boost/test/unit_test.hpp>
#include <graphics/core/input/InputAction.h>
#include <common/PrintHelpers.h>

/**
 * Tests basic construction and field assignment
 */
BOOST_AUTO_TEST_CASE(InputAction_basics)
{
	InputAction action("Jump", InputValueType::SCALAR);
	BOOST_TEST(action.getName() == "Jump");
	BOOST_TEST(action.getValueType() == InputValueType::SCALAR);
}

/**
 * Tests the ability to determine whether two input actions are equal
 */
BOOST_AUTO_TEST_CASE(InputAction_equals)
{
	InputAction action("Jump", InputValueType::VECTOR_2D);
	BOOST_TEST(action == action);
	BOOST_TEST(action != InputAction("Run", InputValueType::VECTOR_2D));
	BOOST_TEST(action != InputAction("Jump", InputValueType::VECTOR_3D));
}