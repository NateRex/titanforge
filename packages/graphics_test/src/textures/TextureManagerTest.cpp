#include <boost/test/unit_test.hpp>
#include <graphics/textures/TextureManager.h>
#include <graphics/textures/Texture.h>
#include <common/exceptions/IllegalArgumentException.h>

/**
 *  Tests that a texture can be constructed, fetched, and destroyed via the manager
 */
BOOST_AUTO_TEST_CASE(TextureManager_basics)
{
	BOOST_REQUIRE_NO_THROW(TextureManager::create("texture", "assets/container.jpg"));
	BOOST_TEST(TextureManager::get("texture") != nullptr);
	BOOST_REQUIRE_NO_THROW(TextureManager::destroy("texture"));
}

/**
 * Tests that an exception is thrown if we try to create a texture that already exists
 */
BOOST_AUTO_TEST_CASE(TextureManager_cannotCreateDuplicates)
{
	TextureManager::create("texture", "assets/container.jpg");
	BOOST_REQUIRE_THROW(TextureManager::create("texture", "assets/container.jpg"), IllegalArgumentException);

	TextureManager::destroy("texture");
}

/**
 * Tests that an exception is thrown if we try to fetch a texture that does not exist
 */
BOOST_AUTO_TEST_CASE(TextureManager_cannotFetchMissing)
{
	BOOST_REQUIRE_THROW(TextureManager::get("does-not-exist"), IllegalArgumentException);
}

/**
 * Tests that an exception is thrown if we try to destroy a texture that does not exist
 */
BOOST_AUTO_TEST_CASE(TextureManager_cannotDestroyMissing)
{
	BOOST_REQUIRE_THROW(TextureManager::destroy("does-not-exist"), IllegalArgumentException);

	TextureManager::create("texture", "assets/container.jpg");
	BOOST_REQUIRE_NO_THROW(TextureManager::destroy("texture"));
	BOOST_REQUIRE_THROW(TextureManager::destroy("texture"), IllegalArgumentException);
}