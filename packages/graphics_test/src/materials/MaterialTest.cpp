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

	TexturePtr texture = TextureLoader::getInstance().load("assets/container.jpg");
	material->texture = texture;
	BOOST_TEST(material->texture == texture);
}