#include <boost/test/unit_test.hpp>
#include <graphics/core/input/InputValue.h>
#include <common/PrintHelpers.h>

/**
 * Tests the ability to store a boolean value
 */
BOOST_AUTO_TEST_CASE(InputValueTest_boolean)
{
	InputValue iv(true);
	BOOST_TEST(iv.getType() == InputValueType::BOOLEAN);
	BOOST_TEST(iv.getBoolean());

	// Copy constructor
	InputValue iv2(iv);
	BOOST_TEST(iv2.getType() == InputValueType::BOOLEAN);
	BOOST_TEST(iv2.getBoolean());
}

/**
 * Tests the ability to store a scalar value
 */
BOOST_AUTO_TEST_CASE(InputValueTest_scalar)
{
	InputValue iv(1.2f);
	BOOST_TEST(iv.getType() == InputValueType::SCALAR);
	BOOST_TEST(iv.getScalar() == 1.2f);

	// Copy constructor
	InputValue iv2(iv);
	BOOST_TEST(iv2.getType() == InputValueType::SCALAR);
	BOOST_TEST(iv2.getScalar() == 1.2f);
}

/**
 * Tests the ability to store a 2D vector value
 */
BOOST_AUTO_TEST_CASE(InputValueTest_vector2D)
{
	Vector2 vector(1.f, 2.f);
	InputValue iv(vector);
	BOOST_TEST(iv.getType() == InputValueType::VECTOR_2D);
	BOOST_TEST(iv.get2D() == vector);

	// Copy constructor
	InputValue iv2(iv);
	BOOST_TEST(iv2.getType() == InputValueType::SCALAR);
	BOOST_TEST(iv2.getScalar() == 1.2f);
}

/**
 * Tests the ability to store a 3D vector value
 */
BOOST_AUTO_TEST_CASE(InputValueTest_vector3D)
{
	Vector3 vector(1.f, 2.f, 3.f);
	InputValue iv(vector);
	BOOST_TEST(iv.getType() == InputValueType::VECTOR_3D);
	BOOST_TEST(iv.get3D() == vector);

	// Copy constructor
	InputValue iv2(iv);
	BOOST_TEST(iv2.getType() == InputValueType::SCALAR);
	BOOST_TEST(iv2.getScalar() == 1.2f);
}