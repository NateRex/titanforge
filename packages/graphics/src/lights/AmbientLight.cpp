#include <graphics/lights/AmbientLight.h>

AmbientLightPtr AmbientLight::create()
{
	return std::shared_ptr<AmbientLight>(new AmbientLight());
}