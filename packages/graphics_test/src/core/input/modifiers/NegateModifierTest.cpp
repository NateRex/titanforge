#include <boost/test/unit_test.hpp>
#include <graphics/core/input/modifiers/NegateModifier.h>
#include <graphics/core/input/InputValue.h>
#include <common/PrintHelpers.h>

/**
 * Tests that booleans, scalars, and the x component of vectors can be negated
 */
BOOST_AUTO_TEST_CASE(NegateModifier_negateX)
{
	NegateModifier mod(true, false, false);
	
	// Affects boolean input
	InputValue result = mod.apply(InputValue(true));
	BOOST_TEST(result.getBoolean() == false);

	// Affects scalar input
	result = mod.apply(InputValue(2.f));
	BOOST_TEST(result.getScalar() == -2.f);

	// Affects x component of a 2D vector
	result = mod.apply(InputValue(1.f, 2.f));
	BOOST_TEST(result.get2D() == Vector2(-1.f, 2.f));

	// Affects x component of a 3D vector
	result = mod.apply(InputValue(1.f, 2.f, 3.f));
	BOOST_TEST(result.get3D() == Vector3(-1.f, 2.f, 3.f));
}

/**
 * Tests that the y component of a vector can be negated
 */
BOOST_AUTO_TEST_CASE(NegateModifier_negateY)
{
	NegateModifier mod(false, true, false);

	// No effect on boolean input
	InputValue result = mod.apply(InputValue(true));
	BOOST_TEST(result.getBoolean());

	// No effect on scalar input
	result = mod.apply(InputValue(2.f));
	BOOST_TEST(result.getScalar() == 2.f);

	// Affects y component of a 2D vector
	result = mod.apply(InputValue(1.f, 2.f));
	BOOST_TEST(result.get2D() == Vector2(1.f, -2.f));

	// Affects y component of a 3D vector
	result = mod.apply(InputValue(1.f, 2.f, 3.f));
	BOOST_TEST(result.get3D() == Vector3(1.f, -2.f, 3.f));
}

/**
 * Tests that the z component of a vector can be negated
 */
BOOST_AUTO_TEST_CASE(NegateModifier_negateZ)
{
	NegateModifier mod(false, false, true);

	// No effect on boolean input
	InputValue result = mod.apply(InputValue(true));
	BOOST_TEST(result.getBoolean());

	// No effect on scalar input
	result = mod.apply(InputValue(2.f));
	BOOST_TEST(result.getScalar() == 2.f);

	// No effect on 2D input
	result = mod.apply(InputValue(1.f, 2.f));
	BOOST_TEST(result.get2D() == Vector2(1.f, 2.f));

	// Affects z component of a 3D vector
	result = mod.apply(InputValue(1.f, 2.f, 3.f));
	BOOST_TEST(result.get3D() == Vector3(1.f, 2.f, -3.f));
}