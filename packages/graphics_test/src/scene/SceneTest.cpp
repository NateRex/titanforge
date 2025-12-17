#include <boost/test/unit_test.hpp>
#include <graphics/scene/Scene.h>
#include <graphics/lights/Light.h>
#include <common/PrintHelpers.h>

/**
 * Tests the basic construction of a scene
 */
BOOST_AUTO_TEST_CASE(Scene_basics)
{
	ScenePtr scene = Scene::create();
	BOOST_TEST(scene->entityType == EntityType::GROUP);

	LightPtr ambientLighting = scene->ambientLighting;
	BOOST_TEST(ambientLighting->color == Color::WHITE);
	BOOST_TEST(ambientLighting->intensity == 0.1f);
}