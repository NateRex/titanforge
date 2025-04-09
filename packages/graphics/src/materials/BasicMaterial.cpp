#include <graphics/materials/BasicMaterial.h>
#include <graphics/textures/Texture.h>

BasicMaterialPtr BasicMaterial::create()
{
	return std::shared_ptr<BasicMaterial>(new BasicMaterial());
}

BasicMaterial::BasicMaterial()
	: Material(MaterialType::BASIC)
{

}