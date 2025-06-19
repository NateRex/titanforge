#include <boost/test/unit_test.hpp>
#include <graphics/core/input/modifiers/SwizzleModifier.h>
#include <graphics/core/input/InputValue.h>
#include <math/Axis.h>
#include <math/Vector2.h>
#include <math/Vector3.h>
#include <common/PrintHelpers.h>

/**
 * Tests that positive boolean values only persist if "X" component is specified for the first
 * position in the swizzle order. Otherwise, the value becomes false.
 */
BOOST_AUTO_TEST_CASE(SwizzleModifier_booleans)
{
	InputValue input(true);

	// Has no effect
	SwizzleModifier mod(Axis::X, Axis::Y, Axis::Z);
	InputValue result = mod.apply(input);
	BOOST_TEST(result.getBoolean());

	// Using y axis for first component results in a false value
	mod = SwizzleModifier(Axis::Y, Axis::X, Axis::Z);
	result = mod.apply(input);
	BOOST_TEST(result.getBoolean() == false);

	// Using z axis for first component results in a false value
	mod = SwizzleModifier(Axis::Z, Axis::X, Axis::Y);
	result = mod.apply(input);
	BOOST_TEST(result.getBoolean() == false);
}

/**
 * Tests that non-zero scalar values only persist if the "X" component is specified for the first
 * position in the swizzle order. Otherwise, the value becomes zero. 
 */
BOOST_AUTO_TEST_CASE(SwizzleModifier_scalars)
{
	InputValue input(1.f);

	// Has no effect
	SwizzleModifier mod(Axis::X, Axis::Y, Axis::Z);
	InputValue result = mod.apply(input);
	BOOST_TEST(result.getScalar() == 1.f);

	// Using y axis for first component results in a zero value
	mod = SwizzleModifier(Axis::Y, Axis::X, Axis::Z);
	result = mod.apply(input);
	BOOST_TEST(result.getScalar() == 0.f);

	// Using y axis for first component results in a zero value
	mod = SwizzleModifier(Axis::Z, Axis::X, Axis::Y);
	result = mod.apply(input);
	BOOST_TEST(result.getScalar() == 0.f);
}

/**
 * Tests the ability to swizzle values of a 2D vector
 */
BOOST_AUTO_TEST_CASE(SwizzleModifier_2d)
{
	InputValue input(1.f, 2.f);

	// Has no effect
	SwizzleModifier mod(Axis::X, Axis::Y, Axis::Z);
	InputValue result = mod.apply(input);
	BOOST_TEST(result.get2D() == Vector2(1.f, 2.f));

	// Swizzle x and y
	mod = SwizzleModifier(Axis::Y, Axis::X, Axis::Z);
	result = mod.apply(input);
	BOOST_TEST(result.get2D() == Vector2(2.f, 1.f));
}

/**
 * Tests that use of the "Z" axis for any component on a 2D vector results in that component becoming zero
 */
BOOST_AUTO_TEST_CASE(SwizzleModifier_2dUsingZ)
{
	InputValue input(1.f, 2.f);

	SwizzleModifier mod(Axis::Z, Axis::Z, Axis::X);
	InputValue result = mod.apply(input);
	BOOST_TEST(result.get2D().isZero());
}

/**
 * Tests the ability to swizzle values of a 3D vector
 */
BOOST_AUTO_TEST_CASE(SwizzleModifier_3d)
{
	InputValue input(1.f, 2.f, 3.f);

	SwizzleModifier mod(Axis::Z, Axis::X, Axis::Y);
	InputValue result = mod.apply(input);
	BOOST_TEST(result.get3D() == Vector3(3.f, 1.f, 2.f));
}