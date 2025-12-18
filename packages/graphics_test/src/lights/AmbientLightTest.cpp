#include <boost/test/unit_test.hpp>
#include <graphics/lights/AmbientLight.h>
#include <common/PrintHelpers.h>
#include <common/exceptions/UnsupportedOperationException.h>

/**
 * Tests the basic construction of an ambient light
 */
BOOST_AUTO_TEST_CASE(AmbientLight_basics)
{
	LightPtr light = AmbientLight::create();
	BOOST_TEST(light->lightType == LightType::AMBIENT);
	BOOST_TEST(light->color == Color::WHITE);
	BOOST_TEST(light->intensity == 1.f);
}

/**
 * Tests exception cases trying to position, rotate, or scale an ambient light
 */
BOOST_AUTO_TEST_CASE(AmbientLight_exceptionCases)
{
	LightPtr light = AmbientLight::create();
	BOOST_REQUIRE_THROW(light->setScaling(1.f, 2.f, 3.f), UnsupportedOperationException);
	BOOST_REQUIRE_THROW(light->setPosition(1.f, 2.f, 3.f), UnsupportedOperationException);
	BOOST_REQUIRE_THROW(light->setRotation(Matrix3::IDENTITY), UnsupportedOperationException);
}