#include <graphics/materials/MeshMaterial.h>

MeshMaterial::MeshMaterial(): Material(MaterialType::MESH)
{
}

MeshMaterialPtr MeshMaterial::create()
{
	return std::shared_ptr<MeshMaterial>(new MeshMaterial());
}

AlphaMode MeshMaterial::getEffectiveAlphaMode() const
{
	if (alphaMode != AlphaMode::AUTO)
	{
		return alphaMode;
	}

	return color.alpha() < 1.f ? AlphaMode::BLEND : AlphaMode::OPAQUE;
}

bool MeshMaterial::isTransparent() const
{
	return getEffectiveAlphaMode() == AlphaMode::BLEND;
}