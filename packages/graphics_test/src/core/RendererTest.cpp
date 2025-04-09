#include <boost/test/unit_test.hpp>
#include <graphics_test/GlobalTestFixture.h>
#include <graphics/core/Renderer.h>
#include <graphics/core/windows/Window.h>
#include <graphics/geometry/BoxGeometry.h>
#include <graphics/materials/BasicMaterial.h>
#include <graphics/entities/Mesh.h>
#include <common/PrintHelpers.h>

/**
 * Tests the ability to get the current renderer time
 */
BOOST_AUTO_TEST_CASE(Renderer_getTime)
{
	Renderer* renderer = GlobalTestFixture::RENDERER;
	BOOST_TEST(renderer->getTime() > 0.0);
}

/**
 * Tests the ability to set and get the background color
 */
BOOST_AUTO_TEST_CASE(Renderer_setBackgroundColor)
{
	Renderer* renderer = GlobalTestFixture::RENDERER;
	Color oldBackground = renderer->getBackgroundColor();

	renderer->setBackgroundColor(Color::BLUE);
	BOOST_TEST(renderer->getBackgroundColor() == Color::BLUE);

	renderer->setBackgroundColor(oldBackground);
}

/**
 * Tests the ability to change the window target
 */
BOOST_AUTO_TEST_CASE(Renderer_setWindow)
{
	Renderer* renderer = GlobalTestFixture::RENDERER;
	WindowPtr window = renderer->getWindow();
	BOOST_TEST(window != nullptr);

	// We won't actually change the window target here to avoid conflicting with other tests
	BOOST_REQUIRE_NO_THROW(renderer->setWindow(window));
}

/**
 * Tests the ability to render an entity
 */
BOOST_AUTO_TEST_CASE(Renderer_render)
{
	// Create simple box mesh
	GeometryPtr geometry = BoxGeometry::create(1.f, 1.f, 1.f);
	MaterialPtr material = BasicMaterial::create();
	MeshPtr mesh = Mesh::create(geometry, material);

	// Render
	Renderer* renderer = GlobalTestFixture::RENDERER;
	BOOST_REQUIRE_NO_THROW((renderer->render(mesh)));
}