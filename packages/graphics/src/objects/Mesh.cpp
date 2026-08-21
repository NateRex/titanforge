#include <graphics/objects/Mesh.h>

Mesh::Mesh(GeometryPtr geometry, MeshMaterialPtr material)
	: Entity(EntityType::MESH), geometry(geometry), material(material)
{

}

MeshPtr Mesh::create(GeometryPtr geometry, MeshMaterialPtr material)
{
	return std::shared_ptr<Mesh>(new Mesh(geometry, material));
}