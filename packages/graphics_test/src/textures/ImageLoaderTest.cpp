#include <boost/test/unit_test.hpp>
#include <graphics/textures/ImageLoader.h>
#include <common/Utils.h>

/**
 * Tests that an image can be loaded
 */
BOOST_AUTO_TEST_CASE(ImageLoader_load)
{
    std::string fullPath = resolvePath("assets/container.jpg");
	int width, height, channels;
	unsigned char* data = stbi_load(fullPath.c_str(), &width, &height, &channels, 0);

    BOOST_TEST(data != nullptr);

    stbi_image_free(data);
}