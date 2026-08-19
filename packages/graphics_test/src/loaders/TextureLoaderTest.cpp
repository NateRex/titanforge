#include <boost/test/unit_test.hpp>
#include <graphics/loaders/TextureLoader.h>
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

	// Loading orientation changes the GPU contents and therefore participates in the cache key.
	TexturePtr flipped = TextureLoader::load("assets/container.jpg", true);
	BOOST_TEST(flipped != texture1);
	BOOST_TEST(TextureLoader::load("assets/container.jpg", true) == flipped);
}
