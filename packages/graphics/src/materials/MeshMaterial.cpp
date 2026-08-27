#include <graphics/materials/MeshMaterial.h>

AlphaMode MeshMaterial::getEffectiveAlphaMode() const
{
	if (alphaMode != AlphaMode::AUTO)
	{
		return alphaMode;
	}

	return color.alpha() < 1.f ? AlphaMode::BLEND : AlphaMode::OPAQUE;
}