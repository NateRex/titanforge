#include <boost/test/unit_test.hpp>
#include <graphics_test/GlobalTestFixture.h>
#include <graphics/core/renderer/Renderer.h>
#include <graphics/core/windows/Window.h>
#include <graphics/scene/Scene.h>
#include <graphics/cameras/PerspectiveCamera.h>
#include <graphics/geometry/BoxGeometry.h>
#include <graphics/materials/MeshMaterial.h>
#include <graphics/materials/PostProcessMaterial.h>
#include <graphics/objects/Mesh.h>
#include <common/PrintHelpers.h>
#include <thread>
#include <chrono>

/**
 * Tests the ability to get the current renderer time
 */
BOOST_AUTO_TEST_CASE(Renderer_getTime)
{
	RendererPtr renderer = GlobalTestFixture::RENDERER;
	BOOST_TEST(renderer->getTime() > 0.0);
}

/**
 * Tests the ability to get the time since the last frame
 */
BOOST_AUTO_TEST_CASE(Renderer_getDeltaTime)
{
	RendererPtr renderer = GlobalTestFixture::RENDERER;
	ScenePtr scene = Scene::create();
	CameraPtr camera = PerspectiveCamera::create(30.f, 800.f / 600.f, 0.1f, 100.f);

	// Without render having been called, time since last frame will be large
	float dt1 = renderer->getDeltaTime();

	// Calling render resets delta time
	renderer->render(scene, camera);
	float dt2 = renderer->getDeltaTime();

	BOOST_TEST(dt2 <= dt1);
}

/**
 * Tests the ability to set and get the background color
 */
BOOST_AUTO_TEST_CASE(Renderer_setBackgroundColor)
{
	RendererPtr renderer = GlobalTestFixture::RENDERER;
	Color oldBackground = renderer->getBackgroundColor();

	renderer->setBackgroundColor(Color::BLUE);
	BOOST_TEST(renderer->getBackgroundColor() == Color::BLUE);

	renderer->setBackgroundColor(oldBackground);
}

/**
 * Tests the ability to render an entity with lighting
 */
BOOST_AUTO_TEST_CASE(Renderer_render)
{
	// Create scene
	ScenePtr scene = Scene::create();

	// Create camera
	CameraPtr camera = PerspectiveCamera::create(30.f, 800.f / 600.f, 0.1f, 100.f);

	// Create simple box mesh and add it to the scene
	GeometryPtr geometry = BoxGeometry::create(1.f, 1.f, 1.f);
	MeshMaterialPtr material = MeshMaterial::create();
	MeshPtr mesh = Mesh::create(geometry, material);
	scene->add(mesh);

	// Render
	RendererPtr renderer = GlobalTestFixture::RENDERER;
	BOOST_REQUIRE_NO_THROW((renderer->render(scene, camera)));
}

/**
 * Tests the convenience path that renders a scene through a post-process material
 */
BOOST_AUTO_TEST_CASE(Renderer_renderWithPostProcessing)
{
	ScenePtr scene = Scene::create();
	CameraPtr camera = PerspectiveCamera::create(30.f, 800.f / 600.f, 0.1f, 100.f);
	PostProcessMaterialPtr material = PostProcessMaterial::create();
	material->exposure = 1.2f;
	material->saturation = 1.1f;
	material->contrast = 1.15f;

	RendererPtr renderer = GlobalTestFixture::RENDERER;
	BOOST_REQUIRE_NO_THROW(renderer->render(scene, camera, material));

	// Ensure the renderer supplies its intermediate texture only for the duration of the draw.
	BOOST_TEST(material->texture == nullptr);
}
