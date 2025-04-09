#include <graphics/entities/Mesh.h>

Mesh::Mesh(GeometryPtr geometry, MaterialPtr material)
	: Entity(EntityType::BUFFERED), geometry(geometry), material(material)
{

}

Mesh::~Mesh()
{
	geometry = nullptr;
	material = nullptr;
}

MeshPtr Mesh::create(GeometryPtr geometry, MaterialPtr material)
{
	return std::shared_ptr<Mesh>(new Mesh(geometry, material));
}