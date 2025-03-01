#include <boost/test/unit_test.hpp>
#include <graphics/Color.h>
#include <common/Utils.h>

/**
 * Tests the construction of a color from floating point values
 */
BOOST_AUTO_TEST_CASE(Color_fromFloats)
{
	Color color = Color::fromFloats(0.1, 0.2, 0.3, 0.4);
	BOOST_TEST(equals(color.red, 0.1, 1.0e-7));
	BOOST_TEST(equals(color.green, 0.2, 1.0e-7));
	BOOST_TEST(equals(color.blue, 0.3, 1.0e-7));
	BOOST_TEST(equals(color.alpha, 0.4, 1.0e-7));
}