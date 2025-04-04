#include <graphics/materials/Material.h>
#include <graphics/textures/Texture.h>

Material::Material(const Color& color, const Texture* texture)
	: color(color), texture(texture)
{

}