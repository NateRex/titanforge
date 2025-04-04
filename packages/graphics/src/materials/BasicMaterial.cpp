#include <graphics/materials/BasicMaterial.h>
#include <graphics/textures/Texture.h>

BasicMaterial::Builder& BasicMaterial::Builder::useVertexColors(bool shouldUseVertexColors)
{
	_useVertexColors = shouldUseVertexColors;
	return *this;
}

BasicMaterialPtr BasicMaterial::Builder::build() const
{
	return std::shared_ptr<BasicMaterial>(new BasicMaterial(_color, _texture, _useVertexColors));
}

BasicMaterial::BasicMaterial(const Color& color, const Texture* texture, bool useVertexColors)
	: Material(MaterialType::BASIC, color, texture), useVertexColors(useVertexColors)
{

}