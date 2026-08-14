#include <graphics/lights/DirectionalLight.h>
#include <math/Vector3.h>
#include <common/exceptions/UnsupportedOperationException.h>

DirectionalLight::DirectionalLight(const Vector3& direction): Light(LightType::DIRECTIONAL)
{
    // Set forward vector
    _rotation.setRow(2, direction);
}

DirectionalLightPtr DirectionalLight::create()
{
    return std::shared_ptr<DirectionalLight>(new DirectionalLight(Vector3::MINUS_YHAT));
}

DirectionalLightPtr DirectionalLight::create(const Vector3& direction)
{
    return std::shared_ptr<DirectionalLight>(new DirectionalLight(direction));
}

void DirectionalLight::updatePosition(float x, float y, float z)
{
    throw UnsupportedOperationException("Position updates are not supported for directional lights");
}