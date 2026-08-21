#include <boost/test/unit_test.hpp>
#include <graphics/materials/PostProcessMaterial.h>
#include <common/PrintHelpers.h>

/**
 * Tests creation and the documented default values of a post-process material.
 */
BOOST_AUTO_TEST_CASE(PostProcessMaterial_defaults)
{
	PostProcessMaterialPtr material = PostProcessMaterial::create();

	BOOST_REQUIRE(material != nullptr);
	BOOST_TEST(material->materialType == MaterialType::POST_PROCESS);
	BOOST_TEST(material->exposure == 1.f);
	BOOST_TEST(material->saturation == 1.f);
	BOOST_TEST(material->contrast == 1.f);
}