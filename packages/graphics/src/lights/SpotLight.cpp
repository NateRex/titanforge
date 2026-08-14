#include <graphics/lights/SpotLight.h>
#include <math/Vector3.h>
#include <math/Matrix3.h>
#include <common/exceptions/IllegalArgumentException.h>

SpotLight::SpotLight(const Vector3& position, const Vector3& direction): Light(LightType::SPOTLIGHT), innerAngle(30.f), outerAngle(45.f)
{
    if (direction.isZero())
    {
        throw IllegalArgumentException("Spotlight direction cannot be zero");
    }

    setPosition(position);
    lookAt(position.plus(direction));
}

SpotLightPtr SpotLight::create()
{
    return std::shared_ptr<SpotLight>(new SpotLight(Vector3::ZERO, Vector3::MINUS_ZHAT));
}

SpotLightPtr SpotLight::create(const Vector3& position, const Vector3& direction)
{
    return std::shared_ptr<SpotLight>(new SpotLight(position, direction));
}
