#include <graphics/scene/Scene.h>

Scene::Scene()
{

}

ScenePtr Scene::create()
{
	return std::shared_ptr<Scene>(new Scene());
}