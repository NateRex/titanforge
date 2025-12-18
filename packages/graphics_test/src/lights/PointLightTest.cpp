#include <boost/test/unit_test.hpp>
#include <graphics/lights/PointLight.h>
#include <common/PrintHelpers.h>
#include <common/exceptions/UnsupportedOperationException.h>

/**
 * Tests the basic construction of a point light
 */
BOOST_AUTO_TEST_CASE(PointLight_basics)
{
	LightPtr light = PointLight::create();
	BOOST_TEST(light->lightType == LightType::POINT);
	BOOST_TEST(light->color == Color::WHITE);
	BOOST_TEST(light->intensity == 1.f);
}

/**
 * Tests exception cases trying to scale a point light
 */
BOOST_AUTO_TEST_CASE(PointLight_exceptionCases)
{
	LightPtr light = PointLight::create();
	BOOST_REQUIRE_THROW(light->setScaling(1.f, 2.f, 3.f), UnsupportedOperationException);
}