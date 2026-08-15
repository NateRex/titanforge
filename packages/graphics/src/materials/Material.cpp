#include <graphics/materials/Material.h>
#include <graphics/textures/Texture.h>

MaterialPtr Material::create()
{
	return std::shared_ptr<Material>(new Material(MaterialType::BASIC));
}

Material::Material(MaterialType type) : materialType(type)
{

}