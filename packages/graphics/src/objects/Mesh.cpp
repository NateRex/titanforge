#include <graphics/objects/Mesh.h>
#include <graphics/materials/MeshMaterial.h>
#include <graphics/core/renderer/RenderState.h>

Mesh::Mesh(EntityType entityType, GeometryPtr geometry, MaterialPtr material): Entity(entityType), geometry(geometry), material(material)
{

}

MeshPtr Mesh::create(GeometryPtr geometry, MaterialPtr material)
{
	return std::shared_ptr<Mesh>(new Mesh(EntityType::MESH, geometry, material));
}

void Mesh::traverse(RenderState& state, const Matrix4& parentModel, const Matrix3& parentNormal)
{
	const Matrix4 modelTransform = parentModel.multiply(getLocalMatrix());
	const Matrix3 normalTransform = parentNormal.multiply(getLocalNormalMatrix());

	RenderItem item;
	item.geometry = geometry.get();
	item.material = material.get();
	item.modelTransform = modelTransform;
	item.normalTransform = normalTransform;
	state.items.push_back(item);

	for (const EntityPtr& child : _children)
	{
		child->traverse(state, modelTransform, normalTransform);
	}
}
