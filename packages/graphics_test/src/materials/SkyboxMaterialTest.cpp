#include <boost/test/unit_test.hpp>
#include <graphics/materials/SkyboxMaterial.h>
#include <common/PrintHelpers.h>

/**
 * Tests creation and the documented default values of a skybox material.
 */
BOOST_AUTO_TEST_CASE(SkyboxMaterial_defaults)
{
	SkyboxMaterialPtr material = SkyboxMaterial::create();

	BOOST_REQUIRE(material != nullptr);
	BOOST_TEST(material->materialType == MaterialType::SKYBOX);
    BOOST_TEST(material->rotation == 0.f);
    BOOST_TEST(material->intensity == 1.f);
    BOOST_TEST(material->lod == 0.f);
    BOOST_TEST(material->color == Color::WHITE);
	BOOST_TEST(material->cullingMode == CullingMode::NONE);
	BOOST_TEST(material->depthWrite == false);
	BOOST_TEST(material->depthFunction == DepthFunction::LESS_OR_EQUAL_TO);
}