#include <graphics/materials/Material.h>
#include <graphics/textures/Texture.h>

Material::Material(MaterialType type, const Color& color, const TexturePtr texture)
	: type(type), color(color), texture(texture)
{

}