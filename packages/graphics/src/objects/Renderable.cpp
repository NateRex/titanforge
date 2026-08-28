#include <graphics/objects/Renderable.h>
#include <graphics/core/renderer/DrawState.h>
#include <graphics/materials/Material.h>
#include <graphics/geometry/Geometry.h>

void Renderable::configureDrawItem(DrawItem& item)
{
	item.geometryBuffer = geometry->getBuffer();
	addMaterialVariant(item, material->shaderId());
	addVertexNormalVariant(item);
}

void Renderable::addMaterialVariant(DrawItem& item, ShaderId shader)
{
	DrawItem::Variant variant;
	variant.mode = RenderModes::MATERIAL;
	variant.shader = shader;
	if (material->isTransparent())
	{
		variant.layer = DrawLayer::TRANSPARENT;
	}
	else if (material->isBackground())
	{
		variant.layer = DrawLayer::BACKGROUND;
	}
	else
	{
		variant.layer = DrawLayer::OPAQUE;
	}
	item.variants.push_back(variant);
}

void Renderable::addVertexNormalVariant(DrawItem& item)
{
	if (geometry->getAttributes().normals)
	{
		DrawItem::Variant vertexNormalVariant;
		vertexNormalVariant.mode = RenderModes::VERTEX_NORMALS;
		vertexNormalVariant.shader = ShaderId::VERTEX_NORMALS;
		vertexNormalVariant.layer = DrawLayer::OPAQUE;
		item.variants.push_back(vertexNormalVariant);
	}
}

void Renderable::traverse(DrawState& state, const Matrix4& parentModel, const Matrix3& parentNormal)
{
    // Compute local-to-world matrices
    const Matrix4 modelTransform = parentModel.multiply(getLocalMatrix());
	const Matrix3 normalTransform = parentNormal.multiply(getLocalNormalMatrix());

	DrawItem item;
	item.material = material.get();
	item.modelTransform = modelTransform;
	item.normalTransform = normalTransform;

	configureDrawItem(item);
	state.items.push_back(item);
	
    // Traverse all children
	for (const EntityPtr& child : _children)
	{
		child->traverse(state, modelTransform, normalTransform);
	}
}