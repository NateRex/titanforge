#include <graphics/materials/PointMaterial.h>

PointMaterial::PointMaterial(): Material(MaterialType::POINT)
{
    cullingMode = CullingMode::NONE;
}

PointMaterialPtr PointMaterial::create()
{
    return std::shared_ptr<PointMaterial>(new PointMaterial());
}