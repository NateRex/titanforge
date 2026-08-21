#include <boost/test/unit_test.hpp>
#include <graphics/materials/MeshMaterial.h>
#include <graphics/loaders/TextureLoader.h>
#include <common/PrintHelpers.h>

/**
 * Tests creation and the documented default values of a mesh material.
 */
BOOST_AUTO_TEST_CASE(MeshMaterial_defaults)
{
	MeshMaterialPtr material = MeshMaterial::create();

	BOOST_REQUIRE(material != nullptr);
	BOOST_TEST(material->materialType == MaterialType::MESH);
	BOOST_TEST(material->alphaMode == AlphaMode::AUTO);
	BOOST_TEST(material->alphaCutoff == 0.5f);
	BOOST_TEST(material->doubleSided == false);
	BOOST_TEST(material->useVertexColors == false);
	BOOST_TEST(material->reflectivity == 0.5f);
	BOOST_TEST(material->shine == 0.6f);
	BOOST_TEST(material->diffuseMap == nullptr);
	BOOST_TEST(material->specularMap == nullptr);
}

/**
 * Tests resolution of alpha mode settings
 */
BOOST_AUTO_TEST_CASE(MeshMaterial_alphaModeResolution)
{
	MeshMaterialPtr material = MeshMaterial::create();
	BOOST_TEST(material->getEffectiveAlphaMode() == AlphaMode::OPAQUE);

	material->color = Color::RED.withAlpha(0.5f);
	BOOST_TEST(material->getEffectiveAlphaMode() == AlphaMode::BLEND);

	material->alphaMode = AlphaMode::MASK;
	material->alphaCutoff = 0.25f;
	BOOST_TEST(material->getEffectiveAlphaMode() == AlphaMode::MASK);
	BOOST_TEST(material->alphaCutoff == 0.25f);
}
