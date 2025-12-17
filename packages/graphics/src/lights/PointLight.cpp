#include <graphics/lights/PointLight.h>

PointLightPtr PointLight::create()
{
	return std::shared_ptr<PointLight>(new PointLight());
}