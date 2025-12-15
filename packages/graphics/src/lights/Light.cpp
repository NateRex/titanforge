#include <graphics/lights/Light.h>

LightPtr Light::create()
{
	return std::shared_ptr<Light>(new Light());
}