#include <graphics/objects/Mesh.h>
#include <graphics/materials/MeshMaterial.h>

Mesh::Mesh(GeometryPtr geometry, MaterialPtr material)
	: Entity(EntityType::MESH), geometry(geometry), material(material)
{

}

MeshPtr Mesh::create(GeometryPtr geometry, MeshMaterialPtr material)
{
	return std::shared_ptr<Mesh>(new Mesh(geometry, material));
}