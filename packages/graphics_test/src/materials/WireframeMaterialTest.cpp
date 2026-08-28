#include <boost/test/unit_test.hpp>
#include <graphics/materials/WireframeMaterial.h>
#include <common/PrintHelpers.h>

/**
 * Tests creation and the documented default values of a wireframe material.
 */
BOOST_AUTO_TEST_CASE(WireframeMaterial_defaults)
{
    WireframeMaterialPtr material = WireframeMaterial::create();

    BOOST_REQUIRE(material != nullptr);
    BOOST_TEST(material->materialType == MaterialType::WIREFRAME);
    BOOST_TEST(material->color == Color::WHITE);
    BOOST_TEST(material->cullingMode == CullingMode::NONE);
    BOOST_TEST(material->depthWrite == true);
    BOOST_TEST(material->depthFunction == DepthFunction::LESS);
}
