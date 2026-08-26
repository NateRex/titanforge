#include <boost/test/unit_test.hpp>
#include <graphics/materials/PointMaterial.h>
#include <common/PrintHelpers.h>

/**
 * Tests creation and the documented default values of a point material.
 */
BOOST_AUTO_TEST_CASE(PointMaterial_defaults)
{
	PointMaterialPtr material = PointMaterial::create();

	BOOST_REQUIRE(material != nullptr);
	BOOST_TEST(material->materialType == MaterialType::POINT);
    BOOST_TEST(material->color == Color::WHITE);
	BOOST_TEST(material->cullingMode == CullingMode::NONE);
	BOOST_TEST(material->depthWrite == true);
	BOOST_TEST(material->depthFunction == DepthFunction::LESS);
	BOOST_TEST(material->sizeUnits == PrimitiveSizeUnits::PIXELS);
	BOOST_TEST(material->size == 6.f);
}
