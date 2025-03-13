#include <boost/test/unit_test.hpp>
#include <graphics/textures/TextureManager.h>
#include <graphics/textures/Texture.h>
#include <common/exceptions/InstantiationException.h>

/**
 * Tests construction and destruction of a texture using the texture manager
 */
BOOST_AUTO_TEST_CASE(Texture_basics)
{
	Texture texture = TextureManager::create("texture", "assets/container.jpg");
	BOOST_TEST(texture.name == "texture");
	BOOST_TEST(texture.imagePath == "assets/container.jpg");

	BOOST_REQUIRE_NO_THROW(TextureManager::destroy("texture"));
}

/**
 * Tests that an instantiation exception is thrown trying to create a texture using an
 * image that does not exist
 */
BOOST_AUTO_TEST_CASE(Texture_nonExistentImage)
{
	BOOST_REQUIRE_THROW(TextureManager::create("texture", "does-not-exist"), InstantiationException);
}