#include <graphics/materials/SkyboxMaterial.h>

SkyboxMaterial::SkyboxMaterial(): Material(MaterialType::SKYBOX, ShaderId::SKYBOX)
{
    cullingMode = CullingMode::NONE;
    depthFunction = DepthFunction::LESS_OR_EQUAL_TO;
    depthWrite = false;
}
