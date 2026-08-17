#include <graphics/materials/Material.h>
#include <graphics/textures/Texture.h>

MaterialPtr Material::create()
{
	return std::shared_ptr<Material>(new Material(MaterialType::DEFAULT));
}

Material::Material(MaterialType type) : materialType(type)
{

}

AlphaMode Material::getEffectiveAlphaMode() const
{
	if (alphaMode != AlphaMode::AUTO)
	{
		return alphaMode;
	}

	return color.alpha() < 1.f ? AlphaMode::BLEND : AlphaMode::OPAQUE;
}
