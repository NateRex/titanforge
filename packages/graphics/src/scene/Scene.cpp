#include <graphics/scene/Scene.h>
#include <graphics/lights/Light.h>

Scene::Scene()
{

}

ScenePtr Scene::create()
{
	return std::shared_ptr<Scene>(new Scene());
}