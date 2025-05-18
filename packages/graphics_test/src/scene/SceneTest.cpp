#include <boost/test/unit_test.hpp>
#include <graphics/scene/Scene.h>
#include <common/PrintHelpers.h>

/**
 * Tests the basic construction of a scene
 */
BOOST_AUTO_TEST_CASE(Scene_basics)
{
	ScenePtr scene = Scene::create();
	BOOST_TEST(scene->type == EntityType::GROUP);
}