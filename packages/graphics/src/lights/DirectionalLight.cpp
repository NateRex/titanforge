#include <graphics/lights/DirectionalLight.h>
#include <math/Vector3.h>
#include <math/Matrix3.h>
#include <common/exceptions/IllegalArgumentException.h>
#include <common/exceptions/UnsupportedOperationException.h>

DirectionalLight::DirectionalLight(const Vector3& direction): Light(LightType::DIRECTIONAL)
{
    if (direction.isZero())
    {
        throw IllegalArgumentException("Directional light direction cannot be zero");
    }

    const Vector3 forward = direction.normalize();

    // Choose a reference axis that is not parallel to forward, then construct
    // the rest of the orthonormal local-to-world basis.
    const Vector3 referenceUp = forward.cross(Vector3::YHAT).isZero(0.00001f)
        ? Vector3::ZHAT
        : Vector3::YHAT;
    const Vector3 right = forward.cross(referenceUp).normalize();
    const Vector3 up = right.cross(forward).normalize();
    _rotation = Matrix3::fromRows(right, up, forward);
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
