#include <graphics/objects/Mesh.h>
#include <graphics/materials/MeshMaterial.h>

Mesh::Mesh(EntityType entityType, GeometryPtr geometry, MaterialPtr material): Entity(entityType), geometry(geometry), material(material)
{

}

MeshPtr Mesh::create(GeometryPtr geometry, MeshMaterialPtr material)
{
	return std::shared_ptr<Mesh>(new Mesh(EntityType::MESH, geometry, material));
}