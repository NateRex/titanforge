#include <boost/test/unit_test.hpp>
#include <graphics/materials/LineMaterial.h>
#include <common/PrintHelpers.h>

/**
 * Tests creation and the documented default values of a line material.
 */
BOOST_AUTO_TEST_CASE(LineMaterial_defaults)
{
	LineMaterialPtr material = LineMaterial::create();

	BOOST_REQUIRE(material != nullptr);
	BOOST_TEST(material->materialType == MaterialType::LINE);
    BOOST_TEST(material->color == Color::WHITE);
	BOOST_TEST(material->cullingMode == CullingMode::NONE);
	BOOST_TEST(material->depthWrite == true);
	BOOST_TEST(material->depthFunction == DepthFunction::LESS);
	BOOST_TEST(material->widthUnits == PrimitiveSizeUnits::PIXELS);
	BOOST_TEST(material->width == 3.f);
}
