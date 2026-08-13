#include <boost/test/unit_test.hpp>
#include <graphics/materials/BasicMaterial.h>
#include <graphics/textures/TextureLoader.h>
#include <common/PrintHelpers.h>

/**
 * Tests the ability to set the public properties of a Material
 */
BOOST_AUTO_TEST_CASE(Material_properties)
{
	MaterialPtr material = BasicMaterial::create();

	material->color = Color::RED;
	BOOST_TEST(material->color == Color::RED);

	TexturePtr img = TextureLoader::load("assets/container.jpg");
	material->texture = img;
	BOOST_TEST(material->texture == img);

	material->diffuseMap = img;
	BOOST_TEST(material->diffuseMap == img);

	material->specularMap = img;
	BOOST_TEST(material->specularMap == img);
}