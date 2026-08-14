#include <graphics/lights/SpotLight.h>
#include <math/Vector3.h>
#include <math/Matrix3.h>
#include <common/exceptions/IllegalArgumentException.h>

SpotLight::SpotLight(const Vector3& position, const Vector3& direction): Light(LightType::SPOTLIGHT)
{
    if (direction.isZero())
    {
        throw IllegalArgumentException("Spotlight direction cannot be zero");
    }

    setPosition(position);
    lookAt(direction);
}