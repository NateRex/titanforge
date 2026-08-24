#include <graphics/materials/SkyboxMaterial.h>

SkyboxMaterial::SkyboxMaterial(): Material(MaterialType::SKYBOX)
{
    
}

SkyboxMaterialPtr SkyboxMaterial::create()
{
    return std::shared_ptr<SkyboxMaterial>(new SkyboxMaterial());
}