#include <graphics/objects/Renderable.h>
#include <graphics/core/renderer/DrawState.h>
#include <graphics/materials/Material.h>
#include <graphics/geometry/Geometry.h>
#include <graphics/geometry/GeometryAttributes.h>

void Renderable::traverse(DrawState& state, const Matrix4& parentModel, const Matrix3& parentNormal)
{
    // Compute local-to-world matrices
    const Matrix4 modelTransform = parentModel.multiply(getLocalMatrix());
	const Matrix3 normalTransform = parentNormal.multiply(getLocalNormalMatrix());

	// Create item
	DrawItem item;
	item.geometry = geometry.get();
	item.material = material.get();
	item.modelTransform = modelTransform;
	item.normalTransform = normalTransform;

	// Add material variant
	DrawItem::Variant materialVariant;
	materialVariant.mode = RenderModes::MATERIAL;
	materialVariant.shader = material->shaderId();
	if (material->isTransparent())
	{
		materialVariant.layer = DrawLayer::TRANSPARENT;
	}
	else if (material->isBackground())
	{
		materialVariant.layer = DrawLayer::BACKGROUND;
	}
	else
	{
		materialVariant.layer = DrawLayer::OPAQUE;
	}
	item.variants.push_back(materialVariant);

	// Add vertex normal variant
	if (geometry->getAttributes().normals)
	{
		DrawItem::Variant vertexNormalVariant;
		vertexNormalVariant.mode = RenderModes::VERTEX_NORMALS;
		vertexNormalVariant.shader = ShaderId::VERTEX_NORMALS;
		vertexNormalVariant.layer = DrawLayer::OPAQUE;
		item.variants.push_back(vertexNormalVariant);
	}

	state.items.push_back(item);
    
    // Traverse all children
	for (const EntityPtr& child : _children)
	{
		child->traverse(state, modelTransform, normalTransform);
	}
}