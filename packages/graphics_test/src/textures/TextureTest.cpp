#include <boost/test/unit_test.hpp>
#include <graphics/textures/Texture.h>
#include <common/exceptions/InstantiationException.h>

/**
 * Tests the basic constructors and accessors of a texture
 */
BOOST_AUTO_TEST_CASE(Texture_basics)
{
	TexturePtr texture = Texture::create("assets/container.jpg");
	BOOST_TEST(texture != nullptr);
	BOOST_TEST(texture->id() > 0);
}

/**
 * Tests that an instantiation exception is thrown trying to create a texture using an image that does not exist
 */
BOOST_AUTO_TEST_CASE(Texture_nonExistentImage)
{
	BOOST_REQUIRE_THROW(Texture::create("does-not-exist"), InstantiationException);
}