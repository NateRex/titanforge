#include <boost/test/unit_test.hpp>
#include <graphics/core/input/InputValue.h>
#include <common/PrintHelpers.h>

/**
 * Tests the ability to store a boolean value
 */
BOOST_AUTO_TEST_CASE(InputValueTest_boolean)
{
	InputValuePtr iv = InputValue::create(true);
	BOOST_TEST(iv->type == InputValueType::BOOLEAN);
	BOOST_TEST(iv->getBoolean());
}

/**
 * Tests the ability to store a scalar value
 */
BOOST_AUTO_TEST_CASE(InputValueTest_scalar)
{
	InputValuePtr iv = InputValue::create(1.2f);
	BOOST_TEST(iv->type == InputValueType::SCALAR);
	BOOST_TEST(iv->getScalar() == 1.2f);
}

/**
 * Tests the ability to store a 2D vector value
 */
BOOST_AUTO_TEST_CASE(InputValueTest_vector2D)
{
	Vector2 vector(1.f, 2.f);
	InputValuePtr iv = InputValue::create(vector);
	BOOST_TEST(iv->type == InputValueType::VECTOR_2D);
	BOOST_TEST(iv->get2D() == vector);
}

/**
 * Tests the ability to store a 3D vector value
 */
BOOST_AUTO_TEST_CASE(InputValueTest_vector3D)
{
	Vector3 vector(1.f, 2.f, 3.f);
	InputValuePtr iv = InputValue::create(vector);
	BOOST_TEST(iv->type == InputValueType::VECTOR_3D);
	BOOST_TEST(iv->get3D() == vector);
}