#include <graphics/entities/Mesh.h>

Mesh::Mesh(GeometryPtr geometry) : _geometry(geometry)
{

}

MeshPtr Mesh::create(GeometryPtr geometry)
{
	return std::shared_ptr<Mesh>(new Mesh(geometry));
}