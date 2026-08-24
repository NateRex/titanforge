#include <graphics/objects/Skybox.h>
#include <graphics/geometry/BoxGeometry.h>
#include <graphics/materials/SkyboxMaterial.h>
#include <common/exceptions/UnsupportedOperationException.h>

Skybox::Skybox(SkyboxMaterialPtr material): Mesh(BoxGeometry::create(2.f, 2.f, 2.f), material)
{

}

SkyboxPtr Skybox::create(SkyboxMaterialPtr material)
{
    return std::shared_ptr<Skybox>(new Skybox(material));
}

void Skybox::updateScaling(float x, float y, float z)
{
    throw UnsupportedOperationException("Scaling updates are not supported for skyboxes");
}

void Skybox::updatePosition(float x, float y, float z)
{
    throw UnsupportedOperationException("Position updates are not supported for skyboxes");
}

void Skybox::updateRotation(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22)
{
    throw UnsupportedOperationException("Rotation updates are not supported for skyboxes");
}