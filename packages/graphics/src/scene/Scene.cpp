#include <graphics/scene/Scene.h>
#include <graphics/lights/Light.h>

Scene::Scene(): ambientLighting(Light::create())
{
	ambientLighting->intensity = 0.1f;
}

ScenePtr Scene::create()
{
	return std::shared_ptr<Scene>(new Scene());
}