#include <boost/test/unit_test.hpp>
#include <graphics/objects/Skybox.h>
#include <graphics/materials/SkyboxMaterial.h>
#include <graphics/textures/TextureCube.h>
#include <common/Utils.h>
#include <common/exceptions/UnsupportedOperationException.h>
#include <common/PrintHelpers.h>

/**
 * Tests creation of the default skybox using embedded imagery, and tests that inherited entity methods for
 * changing position, rotation, and scaling have been disabled.
 */
BOOST_AUTO_TEST_CASE(Skybox_basics)
{
	SkyboxPtr skybox = Skybox::create();
	BOOST_REQUIRE(skybox);

	SkyboxMaterialPtr material = cast<SkyboxMaterial>(skybox->material);
	BOOST_REQUIRE(material);

	TextureCubePtr texture = cast<TextureCube>(material->texture);
	BOOST_REQUIRE(texture);

	BOOST_TEST(texture->size() > 0u);
	BOOST_TEST(texture->format() == PixelFormat::RGB8);

	BOOST_REQUIRE_THROW(skybox->setPosition(0.f, 0.f, 0.f), UnsupportedOperationException);
	BOOST_REQUIRE_THROW(skybox->setScaling(1.f), UnsupportedOperationException);
	BOOST_REQUIRE_THROW(skybox->setRotation(Matrix3::IDENTITY), UnsupportedOperationException);
}

/**
 * Tests creation of a skybox from specified imagery
 */
BOOST_AUTO_TEST_CASE(Skybox_createFromImages)
{
	SkyboxMaterialPtr skyboxMaterial = SkyboxMaterial::create();
	skyboxMaterial->texture = TextureCube::create({
		"assets/skybox/right.jpg",
        "assets/skybox/left.jpg",
        "assets/skybox/top.jpg",
        "assets/skybox/bottom.jpg",
        "assets/skybox/front.jpg",
        "assets/skybox/back.jpg"
	});
	
	SkyboxPtr skybox = Skybox::create(skyboxMaterial);
	BOOST_REQUIRE(skybox);
	BOOST_REQUIRE(skybox->material == skyboxMaterial);
}