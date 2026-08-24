#include <boost/test/unit_test.hpp>
#include <graphics/objects/Skybox.h>
#include <graphics/materials/SkyboxMaterial.h>
#include <graphics/textures/TextureCube.h>
#include <common/Utils.h>
#include <common/PrintHelpers.h>

BOOST_AUTO_TEST_CASE(Skybox_createDefault)
{
	SkyboxPtr skybox = Skybox::create();
	BOOST_REQUIRE(skybox);

	SkyboxMaterialPtr material = cast<SkyboxMaterial>(skybox->material);
	BOOST_REQUIRE(material);
	TextureCubePtr texture = cast<TextureCube>(material->texture);
	BOOST_REQUIRE(texture);
	BOOST_TEST(texture->size() > 0u);
	BOOST_TEST(texture->format() == PixelFormat::RGB8);
}
