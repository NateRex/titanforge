#include <graphics/entities/Mesh.h>

Mesh::Mesh(GeometryPtr geometry, MaterialPtr material)
	: Entity(EntityType::MESH), geometry(geometry), material(material)
{

}

MeshPtr Mesh::create(GeometryPtr geometry, MaterialPtr material)
{
	return std::shared_ptr<Mesh>(new Mesh(geometry, material));
}