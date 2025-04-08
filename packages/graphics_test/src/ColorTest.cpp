#include <boost/test/unit_test.hpp>
#include <graphics/core/Color.h>

/**
 * Tests default construction of a color
 */
BOOST_AUTO_TEST_CASE(Color_default)
{
	Color color;
	BOOST_TEST(color.red() == 0.f);
	BOOST_TEST(color.green() == 0.f);
	BOOST_TEST(color.blue() == 0.f);
	BOOST_TEST(color.alpha() == 1.f);
}

/**
 * Tests construction of a color from floating-point values
 */
BOOST_AUTO_TEST_CASE(Color_fromFloats)
{
	Color color(0.1f, 0.2f, 0.3f, 0.4f);
	BOOST_TEST(color.red() == 0.1f);
	BOOST_TEST(color.green() == 0.2f);
	BOOST_TEST(color.blue() == 0.3f);
	BOOST_TEST(color.alpha() == 0.4f);
}

/**
 * Tests construction of a color from another color
 */
BOOST_AUTO_TEST_CASE(Color_fromColor)
{
	Color color(Color::GREEN);
	BOOST_TEST(color.red() == 0.f);
	BOOST_TEST(color.green() == 1.f);
	BOOST_TEST(color.blue() == 0.f);
	BOOST_TEST(color.alpha() == 1.f);
}

/**
 * Tests the ability to create a color with a different alpha channel value
 */
BOOST_AUTO_TEST_CASE(Color_withAlpha)
{
	Color color = Color::RED.withAlpha(0.5f);
	BOOST_TEST(color.red() == 1.f);
	BOOST_TEST(color.green() == 0.f);
	BOOST_TEST(color.blue() == 0.f);
	BOOST_TEST(color.alpha() == 0.5f);
}