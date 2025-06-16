#include <boost/test/unit_test.hpp>
#include <graphics/core/input/modifiers/InputModifiers.h>
#include <graphics/core/input/modifiers/NegateModifier.h>
#include <graphics/core/input/InputValue.h>
#include <common/PrintHelpers.h>

/**
 * Tests that the ordering of modifiers is respected
 */
BOOST_AUTO_TEST_CASE(InputModifiers_modifierOrder)
{
	InputValue input(1.f, 2.f, 3.f);

	// Negate x before swizzling. Results in negated x value in the y component position.
	InputModifiers mod1;
	mod1.negateX().swizzle(Axis::Y, Axis::X, Axis::Z);
	BOOST_TEST(mod1.size() == 2);
	BOOST_TEST(mod1.apply(input).get3D() == Vector3(2.f, -1.f, 3.f));

	// Negate x after swizzling. Results in negated y value in the x component position.
	InputModifiers mod2;
	mod2.swizzle(Axis::Y, Axis::X, Axis::Z).negateX();
	BOOST_TEST(mod2.size() == 2);
	BOOST_TEST(mod2.apply(input).get3D() == Vector3(-2.f, 1.f, 3.f));
}

/**
 * Tests the ability to apply negations
 */
BOOST_AUTO_TEST_CASE(InputModifiers_negate)
{
	InputValue input(1.f, 2.f, 3.f);

	InputModifiers mod1;
	mod1.negate();
	BOOST_TEST(mod1.apply(input).get3D() == Vector3(-1.f, -2.f, -3.f));

	InputModifiers mod2;
	mod2.negateX();
	BOOST_TEST(mod2.apply(input).get3D() == Vector3(-1.f, 2.f, 3.f));

	InputModifiers mod3;
	mod3.negateY();
	BOOST_TEST(mod3.apply(input).get3D() == Vector3(1.f, -2.f, 3.f));

	InputModifiers mod4;
	mod4.negateZ();
	BOOST_TEST(mod4.apply(input).get3D() == Vector3(1.f, 2.f, -3.f));
}

/**
 * Tests the ability to apply a swizzle
 */
BOOST_AUTO_TEST_CASE(InputModifiers_swizzle)
{
	InputValue input(1.f, 2.f, 3.f);

	InputModifiers mod;
	mod.swizzle(Axis::Y, Axis::Z, Axis::X);
	BOOST_TEST(mod.apply(input).get3D() == Vector3(2.f, 3.f, 1.f));
}