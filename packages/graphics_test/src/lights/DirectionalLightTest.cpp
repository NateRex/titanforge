#include <boost/test/unit_test.hpp>
#include <graphics/lights/DirectionalLight.h>
#include <math/Matrix3.h>
#include <common/Utils.h>
#include <common/PrintHelpers.h>
#include <common/exceptions/IllegalArgumentException.h>
#include <common/exceptions/UnsupportedOperationException.h>

/**
 * Tests the basic construction of an directional light
 */
BOOST_AUTO_TEST_CASE(DirectionalLight_basics)
{
	LightPtr light = DirectionalLight::create();
	BOOST_TEST(light->lightType == LightType::DIRECTIONAL);
	BOOST_TEST(light->color == Color::WHITE);
	BOOST_TEST(light->intensity == 1.f);
    BOOST_TEST(light->getForwardVector() == Vector3::MINUS_YHAT);

    light = DirectionalLight::create(Vector3(0.f, 1.f, 0.f));
    BOOST_TEST(light->getForwardVector() == Vector3::YHAT);

    light->addRotation(Matrix3::fromXRotation(deg2Rad(90.f)));
    BOOST_TEST(light->getForwardVector().equalTo(Vector3::XHAT, 0.00001f));
}

/**
 * Tests exception cases trying to position or scale an directional light
 */
BOOST_AUTO_TEST_CASE(DirectionalLight_exceptionCases)
{
	LightPtr light = DirectionalLight::create();
	BOOST_REQUIRE_THROW(light->setScaling(1.f, 2.f, 3.f), UnsupportedOperationException);
	BOOST_REQUIRE_THROW(light->setPosition(1.f, 2.f, 3.f), UnsupportedOperationException);
	BOOST_REQUIRE_THROW(DirectionalLight::create(Vector3::ZERO), IllegalArgumentException);
}
