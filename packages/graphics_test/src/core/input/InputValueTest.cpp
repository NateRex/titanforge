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
	InputValue iv(1.f, 2.f);

	Vector2 expected(1.f, 2.f);
	BOOST_TEST(iv.getType() == InputValueType::VECTOR_2D);
	BOOST_TEST(iv.get2D() == expected);

	// Copy constructor
	InputValue iv2(iv);
	BOOST_TEST(iv2.getType() == InputValueType::VECTOR_2D);
	BOOST_TEST(iv2.get2D() == expected);
}

/**
 * Tests the ability to store a 3D vector value
 */
BOOST_AUTO_TEST_CASE(InputValueTest_vector3D)
{
	InputValue iv(1.f, 2.f, 3.f);

	Vector3 expected(1.f, 2.f, 3.f);
	BOOST_TEST(iv.getType() == InputValueType::VECTOR_3D);
	BOOST_TEST(iv.get3D() == expected);

	// Copy constructor
	InputValue iv2(iv);
	BOOST_TEST(iv2.getType() == InputValueType::VECTOR_3D);
	BOOST_TEST(iv2.get3D() == expected);
}

/**
 * Tests the ability to construct an input value from an explicit type and 3 component values
 */
BOOST_AUTO_TEST_CASE(InputValueTest_explicit)
{
	// Boolean
	InputValue iv(InputValueType::BOOLEAN, 0.f, 2.f, 3.f);
	BOOST_TEST(iv.getBoolean() == false);

	iv = InputValue(InputValueType::BOOLEAN, 1.f, 2.f, 3.f);
	BOOST_TEST(iv.getBoolean() == true);

	// Scalar
	iv = InputValue(InputValueType::SCALAR, 1.f, 2.f, 3.f);
	BOOST_TEST(iv.getScalar() == 1.f);

	// 2D vector
	iv = InputValue(InputValueType::VECTOR_2D, 1.f, 2.f, 3.f);
	BOOST_TEST(iv.get2D() == Vector2(1.f, 2.f));

	// 3D vector
	iv = InputValue(InputValueType::VECTOR_3D, 1.f, 2.f, 3.f);
	BOOST_TEST(iv.get3D() == Vector3(1.f, 2.f, 3.f));
}