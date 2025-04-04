#include <graphics/materials/MeshMaterial.h>
#include <graphics/textures/Texture.h>

MeshMaterial::Builder& MeshMaterial::Builder::useVertexColors(bool shouldUseVertexColors)
{
	_useVertexColors = shouldUseVertexColors;
	return *this;
}

MeshMaterialPtr MeshMaterial::Builder::build() const
{
	return std::shared_ptr<MeshMaterial>(new MeshMaterial(_color, _texture, _useVertexColors));
}

MeshMaterial::MeshMaterial(const Color& color, const Texture* texture, bool useVertexColors)
	: Material(color, texture), useVertexColors(useVertexColors)
{

}