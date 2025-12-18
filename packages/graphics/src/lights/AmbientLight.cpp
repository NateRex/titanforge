#include <graphics/lights/AmbientLight.h>
#include <common/exceptions/UnsupportedOperationException.h>

AmbientLightPtr AmbientLight::create()
{
	return std::shared_ptr<AmbientLight>(new AmbientLight());
}

void AmbientLight::updatePosition(float x, float y, float z)
{
	throw UnsupportedOperationException("Position updates are not supported for ambient lighting");
}

void AmbientLight::updateRotation(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22)
{
	throw UnsupportedOperationException("Rotation updates are not supported for ambient lighting");
}