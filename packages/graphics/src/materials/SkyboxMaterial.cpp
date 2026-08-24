#include <graphics/materials/SkyboxMaterial.h>

SkyboxMaterial::SkyboxMaterial(): Material(MaterialType::SKYBOX)
{
    cullingMode = CullingMode::NONE;
    depthFunction = DepthFunction::LESS_OR_EQUAL_TO;
    depthWrite = false;
}

SkyboxMaterialPtr SkyboxMaterial::create()
{
    return std::shared_ptr<SkyboxMaterial>(new SkyboxMaterial());
}