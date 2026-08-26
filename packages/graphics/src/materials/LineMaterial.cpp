#include <graphics/materials/LineMaterial.h>

LineMaterial::LineMaterial(): Material(MaterialType::LINE)
{
    cullingMode = CullingMode::NONE;
}

LineMaterialPtr LineMaterial::create()
{
    return std::shared_ptr<LineMaterial>(new LineMaterial());
}