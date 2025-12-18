#include <graphics/lights/Light.h>
#include <common/exceptions/UnsupportedOperationException.h>

void Light::updateScaling(float x, float y, float z)
{
	throw UnsupportedOperationException("Scaling not supported for lights");
}