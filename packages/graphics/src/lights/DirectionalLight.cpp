#include <graphics/lights/DirectionalLight.h>
#include <math/Vector3.h>

DirectionalLight::DirectionalLight(const Vector3& direction): Light(LightType::DIRECTIONAL)
{
    _rotation.setValues(
        1.f, 0.f, 0.f,      // right
        0.f, 1.f, 0.f,      // up
        0.f, -1.f, 0.f
    );
}

DirectionalLightPtr DirectionalLight::create()
{
    return std::shared_ptr<DirectionalLight>(new DirectionalLight());
}