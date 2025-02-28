#include <boost/test/unit_test.hpp>
#include <graphics/Color.h>
#include <common/Utils.h>

/**
 * Tests the construction of a color from floating point values
 */
BOOST_AUTO_TEST_CASE(Color_fromFloats)
{
	Color color = Color::fromFloats(0.1, 0.2, 0.3, 0.4);
	BOOST_TEST(equals(color.getRed(), 0.1, 1.0e-7));
	BOOST_TEST(equals(color.getGreen(), 0.2, 1.0e-7));
	BOOST_TEST(equals(color.getBlue(), 0.3, 1.0e-7));
	BOOST_TEST(equals(color.getAlpha(), 0.4, 1.0e-7));
}