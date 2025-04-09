#include <boost/test/unit_test.hpp>
#include <graphics/textures/TextureLoader.h>
#include <graphics/textures/Texture.h>

/**
 * Tests the ability to load a texture, first from scratch, and then from the cache
 */
BOOST_AUTO_TEST_CASE(TextureManager_load)
{
	TexturePtr texture1 = TextureLoader::load("assets/container.jpg");
	BOOST_TEST(texture1 != nullptr);

	TexturePtr texture2 = TextureLoader::load("assets/container.jpg");
	BOOST_TEST(texture2 == texture1);
}