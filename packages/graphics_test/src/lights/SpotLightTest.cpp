#include <boost/test/unit_test.hpp>
#include <graphics/lights/SpotLight.h>
#include <math/Matrix3.h>
#include <common/Utils.h>
#include <common/PrintHelpers.h>
#include <common/exceptions/IllegalArgumentException.h>
#include <common/exceptions/UnsupportedOperationException.h>

/**
 * Tests the basic construction of an spotlight
 */
BOOST_AUTO_TEST_CASE(SpotLight_basics)
{
	LightPtr light = SpotLight::create();
	BOOST_TEST(light->lightType == LightType::SPOTLIGHT);
	BOOST_TEST(light->color == Color::WHITE);
	BOOST_TEST(light->intensity == 1.f);
	SpotLightPtr spotLight = std::static_pointer_cast<SpotLight>(light);
	BOOST_TEST(spotLight->innerAngle == 5.f);
	BOOST_TEST(spotLight->outerAngle == 15.f);
	BOOST_TEST(spotLight->attenuation);
	BOOST_TEST(spotLight->range == 100.f);
    BOOST_TEST(light->getForwardVector() == Vector3::MINUS_ZHAT);

    light = SpotLight::create(Vector3(1.f, 0.f, 1.f), Vector3::YHAT);
    BOOST_TEST(light->getPosition() == Vector3(1.f, 0.f, 1.f));
    BOOST_TEST(light->getForwardVector() == Vector3::YHAT);

    light->addRotation(Matrix3::fromXRotation(deg2Rad(90.f)));
    BOOST_TEST(light->getForwardVector().equalTo(Vector3::XHAT, 0.00001f));
}

/**
 * Tests exception cases trying to scale a spotlight
 */
BOOST_AUTO_TEST_CASE(SpotLight_exceptionCases)
{
	LightPtr light = SpotLight::create();
	BOOST_REQUIRE_THROW(light->setScaling(1.f, 2.f, 3.f), UnsupportedOperationException);
	BOOST_REQUIRE_THROW(SpotLight::create(Vector3::ZERO, Vector3::ZERO), IllegalArgumentException);
}
