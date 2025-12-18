#include <boost/test/unit_test.hpp>
#include <graphics/materials/BasicMaterial.h>
#include <common/PrintHelpers.h>

/**
 * Tests the ability to set the public properties of a BasicMaterial
 */
BOOST_AUTO_TEST_CASE(BasicMaterial_properties)
{
	BasicMaterialPtr material = BasicMaterial::create();
	BOOST_TEST(material->materialType == MaterialType::BASIC);

	material->useVertexColors = true;
	BOOST_TEST(material->useVertexColors);
}