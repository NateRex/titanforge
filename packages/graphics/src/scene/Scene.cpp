#include <graphics/scene/Scene.h>
#include <graphics/lights/Light.h>

ScenePtr Scene::create()
{
	return std::shared_ptr<Scene>(new Scene());
}