#include <boost/test/unit_test.hpp>
#include <graphics/core/Color.h>
#include <common/Utils.h>

/**
 * Tests basic construction of a color
 */
BOOST_AUTO_TEST_CASE(Color_basics)
{
	// Default
	Color color;
	BOOST_TEST(color.red == 0.f);
	BOOST_TEST(color.green == 0.f);
	BOOST_TEST(color.blue == 0.f);
	BOOST_TEST(color.alpha == 1.f);

	// From floats
	color = Color(0.1f, 0.2f, 0.3f, 0.4f);
	BOOST_TEST(color.red == 0.1f);
	BOOST_TEST(color.green == 0.2f);
	BOOST_TEST(color.blue == 0.3f);
	BOOST_TEST(color.alpha == 0.4f);

	// From another color
	Color color2 = Color(color);
	BOOST_TEST(color2.red == color.red);
	BOOST_TEST(color2.green == color.green);
	BOOST_TEST(color2.blue == color.blue);
	BOOST_TEST(color2.alpha == color.alpha);
}