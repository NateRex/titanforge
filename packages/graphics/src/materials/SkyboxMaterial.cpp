#include <graphics/materials/SkyboxMaterial.h>

SkyboxMaterial::SkyboxMaterial(): Material(MaterialType::SKYBOX)
{
    cullingMode = CullingMode::NONE;
    depthFunction = DepthFunction::LESS_OR_EQUAL_TO;
    depthWrite = false;
}