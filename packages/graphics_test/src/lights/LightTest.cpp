#include <boost/test/unit_test.hpp>
#include <graphics/lights/Light.h>
#include <common/PrintHelpers.h>

/**
 * Tests the basic construction of a light
 */
BOOST_AUTO_TEST_CASE(Light_basics)
{
	LightPtr light = Light::create();
	BOOST_TEST(light->color == Color::WHITE);
	BOOST_TEST(light->intensity == 1.f);
}